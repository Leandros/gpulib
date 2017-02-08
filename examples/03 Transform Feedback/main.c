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

i32 main()
{
  SDL_Window * sdl_window;
  gpu_window("Transform Feedback", 1280, 720, 4, 0, 0, &sdl_window, NULL);

  glDebugMessageCallback(gpu_dbg_msg_cb, NULL);

  v3 * vector1 = gpu_malloc(4 * bytesof(v3));
  v3 * vector2 = gpu_malloc(4 * bytesof(v3));
  v3 * vector3 = gpu_malloc(4 * bytesof(v3));

  vector1[0].x = 1.0f;
  vector1[0].y = 2.0f;
  vector1[0].z = 3.0f;

  vector1[1].x = 4.0f;
  vector1[1].y = 5.0f;
  vector1[1].z = 6.0f;

  vector1[2].x = 7.0f;
  vector1[2].y = 8.0f;
  vector1[2].z = 9.0f;

  vector1[3].x = 10.0f;
  vector1[3].y = 11.0f;
  vector1[3].z = 12.0f;

  vector2[0].x = 13.0f;
  vector2[0].y = 14.0f;
  vector2[0].z = 15.0f;

  vector2[1].x = 16.0f;
  vector2[1].y = 17.0f;
  vector2[1].z = 18.0f;

  vector2[2].x = 19.0f;
  vector2[2].y = 20.0f;
  vector2[2].z = 21.0f;

  vector2[3].x = 22.0f;
  vector2[3].y = 23.0f;
  vector2[3].z = 24.0f;

  let vert_string = gpu_vert_head
      " layout(binding = 0) uniform samplerBuffer s_vector1;     \n"
      " layout(binding = 1) uniform samplerBuffer s_vector2;     \n"
      "                                                          \n"
      " out vec3 vector3;                                        \n"
      "                                                          \n"
      " void main()                                              \n"
      " {                                                        \n"
      "   vec3 vector1 = texelFetch(s_vector1, gl_VertexID).xyz; \n"
      "   vec3 vector2 = texelFetch(s_vector2, gl_VertexID).xyz; \n"
      "                                                          \n"
      "   vector3 = vector1 + vector2;                           \n"
      " }                                                        \n";

  let compute_vert = gpu_vert_xfb(vert_string, 1, (const char * []){"vector3"});

  let xfb = gpu_xfb(vector3, 0, 4 * bytesof(v3), NULL, 0, 0, NULL, 0, 0, NULL, 0, 0);
  let ppo = gpu_ppo(compute_vert, 0);

  let vector1_tex = gpu_cast(vector1, gpu_xyz_f32_t, 0, 4 * bytesof(v3));
  let vector2_tex = gpu_cast(vector2, gpu_xyz_f32_t, 0, 4 * bytesof(v3));

  u32 textures[] =
  {
    [0] = vector1_tex,
    [1] = vector2_tex
  };

  struct gpu_op_t ops[] =
  {
    [0].tex_count = countof(textures),
    [0].tex = textures,
    [0].mode = gpu_points_t,
    [0].ppo = ppo,
    [0].cmd_count = 1,
    [0].cmd = (struct gpu_cmd_t []){[0].count = 4, [0].instance_count = 1}
  };

  gpu_bind_xfb(xfb);
  gpu_draw_xfb(countof(ops), ops);
  gpu_bind_xfb(0);

  glFinish();

  char print_str[10000] = {};
  SDL_snprintf(
      print_str, 10000,
      "vector3[0].xyz: %f %f %f\n"
      "vector3[1].xyz: %f %f %f\n"
      "vector3[2].xyz: %f %f %f\n"
      "vector3[3].xyz: %f %f %f\n",
      vector3[0].x, vector3[0].y, vector3[0].z,
      vector3[1].x, vector3[1].y, vector3[1].z,
      vector3[2].x, vector3[2].y, vector3[2].z,
      vector3[3].x, vector3[3].y, vector3[3].z);

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Completed", print_str, NULL);

exit:
  return 0;
}
