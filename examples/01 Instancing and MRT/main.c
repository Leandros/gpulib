#include "../../gpulib.h"

typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;
typedef uint16_t f16;
typedef float    f32;
typedef double   f64;

#define var __auto_type
#define let __auto_type const
#define case break; case
#define default break; default
#define bytesof(x) (ptrdiff_t)(sizeof(x))
#define countof(x) (ptrdiff_t)(sizeof(x) / sizeof((x)[0]))

static void gpu_dbg_msg_cb(
    uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char * message, void * userdata)
{
  const char * GL_ERROR_SOURCE[] =
  {
    "API",
    "WINDOW SYSTEM",
    "SHADER COMPILER",
    "THIRD PARTY",
    "APPLICATION",
    "OTHER"
  };

  const char * GL_ERROR_SEVERITY[] =
  {
    "HIGH",
    "MEDIUM",
    "LOW",
    "NOTIFICATION"
  };

  const char * GL_ERROR_TYPE[] =
  {
    "ERROR",
    "DEPRECATED BEHAVIOR",
    "UNDEFINED DEHAVIOUR",
    "PORTABILITY",
    "PERFORMANCE",
    "OTHER"
  };

  SDL_Log(
    "\n" "OPENGL DEBUG"
    "\n" "ID:       %u"
    "\n" "SOURCE:   %s"
    "\n" "SEVERITY: %s"
    "\n" "TYPE:     %s"
    "\n" "MESSAGE:  %s"
    "\n", id,
    GL_ERROR_SOURCE[source - 0x8246], // GL_DEBUG_SOURCE_API
    GL_ERROR_SEVERITY[
      severity != 0x826B ? // GL_DEBUG_SEVERITY_NOTIFICATION
      severity  - 0x9146 : 3], // GL_DEBUG_SEVERITY_HIGH
    GL_ERROR_TYPE[type - 0x824C], // GL_DEBUG_TYPE_ERROR
    message);
}

typedef struct
{
  f32 x, y, z;
} v3;

typedef struct
{
  f32 x, y, z, w;
} v4;

#define MAX_STR 10000

union
{
  char c[0];
  struct
  {
    char monkey_ibo [MAX_STR];
    char sphere_ibo [MAX_STR];
    char teapot_ibo [MAX_STR];
    char texture_1  [MAX_STR];
    char texture_2  [MAX_STR];
    char texture_3  [MAX_STR];
    char cubemap_px [MAX_STR];
    char cubemap_nx [MAX_STR];
    char cubemap_py [MAX_STR];
    char cubemap_ny [MAX_STR];
    char cubemap_pz [MAX_STR];
    char cubemap_nz [MAX_STR];
    char vs_cube    [MAX_STR];
    char fs_cube    [MAX_STR];
    char vs_mesh    [MAX_STR];
    char fs_mesh    [MAX_STR];
    char vs_quad    [MAX_STR];
    char fs_quad    [MAX_STR];
  };
} RESRC =
{
  .monkey_ibo = "data/meshes/monkey.ibo",
  .sphere_ibo = "data/meshes/sphere.ibo",
  .teapot_ibo = "data/meshes/teapot.ibo",
  .texture_1  = "data/textures/texture_1.bmp",
  .texture_2  = "data/textures/texture_2.bmp",
  .texture_3  = "data/textures/texture_3.bmp",
  .cubemap_px = "data/textures/cubemap_right.bmp",
  .cubemap_nx = "data/textures/cubemap_left.bmp",
  .cubemap_py = "data/textures/cubemap_top.bmp",
  .cubemap_ny = "data/textures/cubemap_bottom.bmp",
  .cubemap_pz = "data/textures/cubemap_front.bmp",
  .cubemap_nz = "data/textures/cubemap_back.bmp",
  .vs_cube    = "shaders/cubemap.vert",
  .fs_cube    = "shaders/cubemap.frag",
  .vs_mesh    = "shaders/mesh.vert",
  .fs_mesh    = "shaders/mesh.frag",
  .vs_quad    = "shaders/quad.vert",
  .fs_quad    = "shaders/quad.frag"
};

static inline void ReadIBO(const char * ibo_filepath, i64 * bytes, v4 * data)
{
  SDL_RWops * fd = SDL_RWFromFile(ibo_filepath, "rb");

  SDL_assert(fd != NULL);

  SDL_RWseek(fd, 0, RW_SEEK_END);
  *bytes = SDL_RWtell(fd);
  SDL_RWseek(fd, 0, RW_SEEK_SET);

  if (data)
    SDL_RWread(fd, data, *bytes, 1);

  SDL_RWclose(fd);
}

static inline v3 v3addv4(v3 a, v4 b)
{
  return (v3)
  {
    a.x + b.x,
    a.y + b.y,
    a.z + b.z
  };
}

static inline v3 v3subv4(v3 a, v4 b)
{
  return (v3)
  {
    a.x - b.x,
    a.y - b.y,
    a.z - b.z
  };
}

static inline v4 vermul(v4 a, v4 b)
{
  return (v4)
  {
    a.x * b.w + b.x * a.w + (a.y * b.z - b.y * a.z),
    a.y * b.w + b.y * a.w + (a.z * b.x - b.z * a.x),
    a.z * b.w + b.z * a.w + (a.x * b.y - b.x * a.y),
    a.w * b.w - (a.x * b.x + a.y * b.y + a.z * b.z)
  };
}

static inline v4 verneg(v4 v)
{
  return (v4){-v.x, -v.y, -v.z, v.w};
}

static inline v4 v4rot(v4 p, v4 v)
{
  return vermul(vermul(v, p), verneg(v));
}

static inline f32 sinfdegdiv2(f32 d)
{
  return sin(d * (M_PI / 180.f) / 2.f);
}

static inline f32 cosfdegdiv2(f32 d)
{
  return cos(d * (M_PI / 180.f) / 2.f);
}

static inline f32 tanfdegdiv2(f32 d)
{
  return tan(d * (M_PI / 180.f) / 2.f);
}

i32 main()
{
  let path_exe = SDL_GetBasePath();

  for (int i = 0, size = bytesof(RESRC); i < size; i += MAX_STR)
  {
    char path_res[MAX_STR];
    SDL_memcpy(path_res, &RESRC.c[i], MAX_STR);
    SDL_snprintf(&RESRC.c[i], MAX_STR, "%s%s", path_exe, path_res);
  }

  SDL_Window * sdl_window;
  gpu_window("Instancing and MRT", 1280, 720, 4, 0, 0, &sdl_window, NULL);

  glDebugMessageCallback(gpu_dbg_msg_cb, NULL);

  i64 monkey_bytes = 0;
  i64 sphere_bytes = 0;
  i64 teapot_bytes = 0;
  ReadIBO(RESRC.monkey_ibo, &monkey_bytes, NULL);
  ReadIBO(RESRC.sphere_ibo, &sphere_bytes, NULL);
  ReadIBO(RESRC.teapot_ibo, &teapot_bytes, NULL);

  let meshes = gpu_malloc(monkey_bytes + sphere_bytes + teapot_bytes);

  v4 * monkey_mesh = meshes;
  v4 * sphere_mesh = meshes;
  v4 * teapot_mesh = meshes;
  u32 monkey_mesh_tex = 0;
  u32 sphere_mesh_tex = 0;
  u32 teapot_mesh_tex = 0;
  {
    var i = 0;
    monkey_mesh += i / bytesof(v4); monkey_mesh_tex = gpu_cast(meshes, gpu_xyzw_f32_t, i, monkey_bytes); i += monkey_bytes;
    sphere_mesh += i / bytesof(v4); sphere_mesh_tex = gpu_cast(meshes, gpu_xyzw_f32_t, i, sphere_bytes); i += sphere_bytes;
    teapot_mesh += i / bytesof(v4); teapot_mesh_tex = gpu_cast(meshes, gpu_xyzw_f32_t, i, teapot_bytes);
  }

  ReadIBO(RESRC.monkey_ibo, &monkey_bytes, monkey_mesh);
  ReadIBO(RESRC.sphere_ibo, &sphere_bytes, sphere_mesh);
  ReadIBO(RESRC.teapot_ibo, &teapot_bytes, teapot_mesh);

  struct gpu_cmd_t monkey_cmds[] = {[0].count = (i32)monkey_bytes / bytesof(v4) / 2, [0].instance_count = 30};
  struct gpu_cmd_t sphere_cmds[] = {[0].count = (i32)sphere_bytes / bytesof(v4) / 2, [0].instance_count = 30};
  struct gpu_cmd_t teapot_cmds[] = {[0].count = (i32)teapot_bytes / bytesof(v4) / 2, [0].instance_count = 30};

  v3 * pos = gpu_malloc(90 * bytesof(v3));

  for (int i = 0, row = 10, space = 3; i < 90; ++i)
  {
    pos[i].x = (f32)(i * space - (i / row) * row * space);
    pos[i].y = 0;
    pos[i].z = (f32)((i / row) * space);
  }

  let monkey_pos_tex = gpu_cast(pos, gpu_xyz_f32_t, 00 * bytesof(v3), 30 * bytesof(v3));
  let sphere_pos_tex = gpu_cast(pos, gpu_xyz_f32_t, 30 * bytesof(v3), 30 * bytesof(v3));
  let teapot_pos_tex = gpu_cast(pos, gpu_xyz_f32_t, 60 * bytesof(v3), 30 * bytesof(v3));

  const char * tex[] =
  {
    RESRC.texture_1,
    RESRC.texture_2,
    RESRC.texture_3
  };

  const char * cbm_px[] = {RESRC.cubemap_px};
  const char * cbm_nx[] = {RESRC.cubemap_nx};
  const char * cbm_py[] = {RESRC.cubemap_py};
  const char * cbm_ny[] = {RESRC.cubemap_ny};
  const char * cbm_pz[] = {RESRC.cubemap_pz};
  const char * cbm_nz[] = {RESRC.cubemap_nz};

  let textures = gpu_malloc_img(gpu_srgb_b8_t, 512, 512, countof(tex), 4);
  let skyboxes = gpu_malloc_cbm(gpu_srgb_b8_t, 512, 512, countof(cbm_px), 4);

  gpu_bmp_img(textures, 512, 512, countof(tex), tex);
  gpu_bmp_cbm(skyboxes, 512, 512, countof(cbm_px), cbm_px, cbm_nx, cbm_py, cbm_ny, cbm_pz, cbm_nz);

  gpu_mip(textures);
  gpu_mip(skyboxes);

  let monkey_tex = gpu_cast_img(textures, gpu_srgb_b8_t, 0, 1, 0, 4);
  let sphere_tex = gpu_cast_img(textures, gpu_srgb_b8_t, 1, 1, 0, 4);
  let teapot_tex = gpu_cast_img(textures, gpu_srgb_b8_t, 2, 1, 0, 4);

  let mrt_depth = gpu_malloc_img(gpu_d_f32_t, 1280, 720, 1, 1);
  let mrt_color = gpu_malloc_img(gpu_srgba_b8_t, 1280, 720, 1, 1);

  let mrt_fbo = gpu_fbo(mrt_color, 0, 0, 0, 0, 0, 0, 0, mrt_depth, 0);

  u32 monkey_textures[] =
  {
    [0] = monkey_mesh_tex,
    [1] = monkey_pos_tex,
    [2] = monkey_tex
  };

  u32 sphere_textures[] =
  {
    [0] = sphere_mesh_tex,
    [1] = sphere_pos_tex,
    [2] = sphere_tex
  };

  u32 teapot_textures[] =
  {
    [0] = teapot_mesh_tex,
    [1] = teapot_pos_tex,
    [2] = teapot_tex
  };

  u32 other_textures[] =
  {
    [3] = skyboxes,
    [4] = mrt_color
  };

  let smp_textures = gpu_smp(4, gpu_linear_mipmap_linear_t, gpu_linear_t, gpu_repeat_t);
  let smp_mrtcolor = gpu_smp(1, gpu_nearest_t, gpu_nearest_t, gpu_clamp_to_border_t);

  u32 mesh_samplers[] =
  {
    [2] = smp_textures
  };

  u32 other_samplers[] =
  {
    [3] = smp_textures,
    [4] = smp_mrtcolor
  };

  let mesh_vert = gpu_vert_file(RESRC.vs_mesh);
  let mesh_frag = gpu_frag_file(RESRC.fs_mesh);

  let quad_vert = gpu_vert_file(RESRC.vs_quad);
  let quad_frag = gpu_frag_file(RESRC.fs_quad);

  let cube_vert = gpu_vert_file(RESRC.vs_cube);
  let cube_frag = gpu_frag_file(RESRC.fs_cube);

  let mesh_ppo = gpu_ppo(mesh_vert, mesh_frag);
  let quad_ppo = gpu_ppo(quad_vert, quad_frag);
  let cube_ppo = gpu_ppo(cube_vert, cube_frag);

  struct gpu_op_t ops[] =
  {
    [0].vert = mesh_vert, [0].frag = mesh_frag, [0].ppo = mesh_ppo, [0].smp_count = countof(mesh_samplers), [0].smp = mesh_samplers, [0].mode = gpu_triangles_t,
    [1].vert = mesh_vert, [1].frag = mesh_frag, [1].ppo = mesh_ppo, [1].smp_count = countof(mesh_samplers), [1].smp = mesh_samplers, [1].mode = gpu_triangles_t,
    [2].vert = mesh_vert, [2].frag = mesh_frag, [2].ppo = mesh_ppo, [2].smp_count = countof(mesh_samplers), [2].smp = mesh_samplers, [2].mode = gpu_triangles_t,

    [0].id = 0,
    [0].tex_count = countof(monkey_textures),
    [0].cmd_count = countof(monkey_cmds),
    [0].tex = monkey_textures,
    [0].cmd = monkey_cmds,

    [1].id = 1,
    [1].tex_count = countof(sphere_textures),
    [1].cmd_count = countof(sphere_cmds),
    [1].tex = sphere_textures,
    [1].cmd = sphere_cmds,

    [2].id = 2,
    [2].tex_count = countof(teapot_textures),
    [2].cmd_count = countof(teapot_cmds),
    [2].tex = teapot_textures,
    [2].cmd = teapot_cmds
  };

  struct gpu_op_t skybox_ops[] =
  {
    [0].tex_count = countof(other_textures),
    [0].smp_count = countof(other_samplers),
    [0].tex = other_textures,
    [0].smp = other_samplers,
    [0].ppo = cube_ppo,
    [0].mode = gpu_triangles_t,
    [0].cmd_count = 1,
    [0].cmd = (struct gpu_cmd_t []){[0].count = 36, [0].instance_count = 1}
  };

  struct gpu_op_t mrt_ops[] =
  {
    [0].tex_count = countof(other_textures),
    [0].smp_count = countof(other_samplers),
    [0].tex = other_textures,
    [0].smp = other_samplers,
    [0].ppo = quad_ppo,
    [0].mode = gpu_triangles_t,
    [0].cmd_count = 1,
    [0].cmd = (struct gpu_cmd_t []){[0].count = 6, [0].instance_count = 1}
  };

  v3 cam_pos = {23.518875f, 5.673130f, 26.64900f};
  v4 cam_rot = {-0.351835f, 0.231701f, 0.090335f, 0.902411f};

  f32 fov = 1.f / tanfdegdiv2(85.f);
  f32 fov_x = fov / (1280 / 720.f);
  f32 fov_y = fov;

  SDL_SetRelativeMouseMode(1);
  u32 t_prev = SDL_GetTicks();

  while (1)
  {
    for (SDL_Event e; SDL_PollEvent(&e);)
    {
      if (e.type == SDL_QUIT)
        goto exit;
    }

    u32 t_curr = SDL_GetTicks();
    f64 dt = ((t_curr - t_prev) * 60.0) / 1000.0;

    SDL_PumpEvents();
    const unsigned char * const key = SDL_GetKeyboardState(0);

    i32 mx_i32 = 0;
    i32 my_i32 = 0;
    SDL_GetRelativeMouseState(&mx_i32, &my_i32);
    f32 mx = mx_i32 * -0.05f;
    f32 my = my_i32 * -0.05f;
    f32 sx = sinfdegdiv2(mx);
    f32 cx = cosfdegdiv2(mx);
    f32 sy = sinfdegdiv2(my);
    f32 cy = cosfdegdiv2(my);
    cam_rot = vermul(cam_rot, ((v4){sy, 0.f, 0.f, cy}));
    cam_rot = vermul(((v4){0.f, sx, 0.f, cx}), cam_rot);

    if (key[SDL_SCANCODE_D]) cam_pos = v3addv4(cam_pos, v4rot(((v4){0.05f, 0.f, 0.f}), cam_rot));
    if (key[SDL_SCANCODE_A]) cam_pos = v3subv4(cam_pos, v4rot(((v4){0.05f, 0.f, 0.f}), cam_rot));
    if (key[SDL_SCANCODE_E]) cam_pos = v3addv4(cam_pos, v4rot(((v4){0.f, 0.05f, 0.f}), cam_rot));
    if (key[SDL_SCANCODE_Q]) cam_pos = v3subv4(cam_pos, v4rot(((v4){0.f, 0.05f, 0.f}), cam_rot));
    if (key[SDL_SCANCODE_S]) cam_pos = v3addv4(cam_pos, v4rot(((v4){0.f, 0.f, 0.05f}), cam_rot));
    if (key[SDL_SCANCODE_W]) cam_pos = v3subv4(cam_pos, v4rot(((v4){0.f, 0.f, 0.05f}), cam_rot));

    static i32 show_pass = 0;
    if (key[SDL_SCANCODE_0]) show_pass = 0;
    if (key[SDL_SCANCODE_1]) show_pass = 1;
    if (key[SDL_SCANCODE_2]) show_pass = 2;
    if (key[SDL_SCANCODE_3]) show_pass = 3;
    if (key[SDL_SCANCODE_4]) show_pass = 4;
    if (key[SDL_SCANCODE_5]) show_pass = 5;
    if (key[SDL_SCANCODE_6]) show_pass = 6;
    if (key[SDL_SCANCODE_7]) show_pass = 7;
    if (key[SDL_SCANCODE_8]) show_pass = 8;

    for (int i = 0; i < 90; ++ i)
      pos[i].y = (f32)sin(t_curr * 0.0015f + i * 0.5f) * 0.3f;

    gpu_v_3(mesh_vert, 1, 1, &cam_pos.x);
    gpu_v_4(mesh_vert, 2, 1, &cam_rot.x);
    gpu_f32(mesh_vert, 3, 1, &fov_x);
    gpu_f32(mesh_vert, 4, 1, &fov_y);

    gpu_v_3(mesh_frag, 1, 1, &cam_pos.x);
    gpu_i32(mesh_frag, 2, 1, &show_pass);

    gpu_v_4(cube_vert, 0, 1, &cam_rot.x);
    gpu_f32(cube_vert, 1, 1, &fov_x);
    gpu_f32(cube_vert, 2, 1, &fov_y);

    f32 t = (f32)t_curr / 1000.f;
    gpu_f32(quad_frag, 0, 1, &t);

    gpu_bind_fbo(mrt_fbo);
    gpu_clear();
    gpu_draw(countof(ops), ops);
    gpu_bind_fbo(0);

    gpu_clear();

    if (!show_pass)
    {
      glDisable(gpu_depth_t);
      gpu_draw(countof(skybox_ops), skybox_ops);
      glEnable(gpu_depth_t);
    }

    gpu_draw(countof(mrt_ops), mrt_ops);

    gpu_swap(sdl_window);

    t_prev = t_curr;
  }

exit:
  return 0;
}
