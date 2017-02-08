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
  f32 x, y, z, w;
} v4;

i32 main()
{
  SDL_Window * sdl_window = NULL;
  gpu_window("GPGPU", 1280, 720, 4, 0, 0, &sdl_window, NULL);

  glDebugMessageCallback(gpu_dbg_msg_cb, NULL);

  let dim_x = 4096;
  let dim_y = 4096;
  let dim = dim_x * dim_y;

  f32 * mat_1 = gpu_malloc(dim * bytesof(f32));
  f32 * mat_2 = gpu_malloc(dim * bytesof(f32));

  for (int i = 0; i < dim; ++i) mat_1[i] = i + 1.f;
  for (int i = 0; i < dim; ++i) mat_2[i] = i + 1.f;

  let vert_string =
      gpu_vert_head " const vec2 quad[] = vec2[]                     \n"
                    " (                                              \n"
                    "   vec2( -1.0, -1.0 ),                          \n"
                    "   vec2(  1.0, -1.0 ),                          \n"
                    "   vec2( -1.0,  1.0 ),                          \n"
                    "   vec2( -1.0,  1.0 ),                          \n"
                    "   vec2(  1.0, -1.0 ),                          \n"
                    "   vec2(  1.0,  1.0 )                           \n"
                    " );                                             \n"
                    "                                                \n"
                    " void main()                                    \n"
                    " {                                              \n"
                    "   gl_Position = vec4(quad[gl_VertexID], 0, 1); \n"
                    " }                                              \n";

  let frag_string = gpu_frag_head
      " layout(binding = 0) uniform samplerBuffer s_mat_1;               \n"
      " layout(binding = 1) uniform samplerBuffer s_mat_2;               \n"
      "                                                                  \n"
      " layout(location = 0) out vec4 fbo_color;                         \n"
      "                                                                  \n"
      " void main()                                                      \n"
      " {                                                                \n"
      "   int x_coord = int(floor(gl_FragCoord.x));                      \n"
      "   int y_coord = int(floor(gl_FragCoord.y));                      \n"
      "                                                                  \n"
      "   float mat_1 = texelFetch(s_mat_1, y_coord * 4096 + x_coord).x; \n"
      "   float mat_2 = texelFetch(s_mat_2, y_coord * 4096 + x_coord).x; \n"
      "                                                                  \n"
      "   float mat_sum = mat_1 + mat_2;                                 \n"
      "                                                                  \n"
      "   fbo_color = vec4(mat_sum, 0, 0, 1);                            \n"
      " }                                                                \n";

  let vert = gpu_vert(vert_string);
  let frag = gpu_frag(frag_string);
  let ppo = gpu_ppo(vert, frag);

  let img_tex = gpu_malloc_img(gpu_rgba_f32_t, dim_x, dim_y, 1, 1);
  let fbo = gpu_fbo(img_tex, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  u32 mat_1_tex = gpu_cast(mat_1, gpu_x_f32_t, 0, dim * bytesof(f32));
  u32 mat_2_tex = gpu_cast(mat_2, gpu_x_f32_t, 0, dim * bytesof(f32));

  u32 textures[] =
  {
    [0] = mat_1_tex,
    [1] = mat_2_tex
  };

  struct gpu_cmd_t cmd[] =
  {
    [0].count = 6,
    [0].instance_count = 1
  };

  struct gpu_op_t ops[] =
  {
    [0].tex_count = countof(textures),
    [0].tex = textures,
    [0].ppo = ppo,
    [0].mode = gpu_triangles_t,
    [0].cmd_count = countof(cmd),
    [0].cmd = cmd
  };

  gpu_clear();
  SDL_GL_SwapWindow(sdl_window);

  i32 win_w, win_h;
  SDL_GetWindowSize(sdl_window, &win_w, &win_h);

  glViewport(0, 0, dim_x, dim_y);
  gpu_bind_fbo(fbo);
  {
    gpu_clear();
    gpu_draw(countof(ops), ops);
  }
  gpu_bind_fbo(0);
  glViewport(0, 0, win_w, win_h);

  let pixels_bytes = dim * bytesof(v4);

  void * pix = SDL_malloc((size_t)pixels_bytes);

  gpu_get(img_tex, 0, 0, 0, dim_x, dim_y, gpu_rgba_t, gpu_f32_t, pixels_bytes, pix);

  v4 * out = pix;

  char print_str[10000] = {};
  SDL_snprintf(
      print_str, 10000, "pixels[%d].rgba: %f %f %f %f\n", dim - 1,
      out[dim - 1].x, out[dim - 1].y, out[dim - 1].z, out[dim - 1].w);

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Completed", print_str, NULL);
  SDL_free(pix);

  return 0;
}
