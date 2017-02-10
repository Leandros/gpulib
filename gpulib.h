#pragma once

#include "SDL2/SDL.h"
#include "gl_functions.h"
#include "tinyprofiler.h"

struct gpu_cmd_t
{
  int count;
  int instance_count;
  int first;
  int instance_first;
};

struct gpu_op_t
{
  int id;
  int tex_first;
  int tex_count;
  int smp_first;
  int smp_count;
  uint32_t * tex;
  uint32_t * smp;
  uint32_t vert;
  uint32_t frag;
  uint32_t ppo;
  uint32_t mode;
  int cmd_count;
  struct gpu_cmd_t * cmd;
};

enum gpu_draw_t
{
  gpu_lines_t     = 0x0001, // GL_LINES
  gpu_points_t    = 0x0000, // GL_POINTS
  gpu_triangles_t = 0x0004  // GL_TRIANGLES
};

enum gpu_shader_t
{
  gpu_frag_t = 0x8B30, // GL_FRAGMENT_SHADER
  gpu_vert_t = 0x8B31  // GL_VERTEX_SHADER
};

enum gpu_global_t
{
  gpu_cull_t     = 0x0B44, // GL_CULL_FACE
  gpu_depth_t    = 0x0B71, // GL_DEPTH_TEST
  gpu_scissor_t  = 0x0C11, // GL_SCISSOR_TEST
  gpu_fbo_srgb_t = 0x8DB9  // GL_FRAMEBUFFER_SRGB
};

enum gpu_tex_mem_format_t
{
  gpu_x_b8_t     = 0x8229, // GL_R8
  gpu_x_f16_t    = 0x822D, // GL_R16F
  gpu_x_f32_t    = 0x822E, // GL_R32F
  gpu_x_i8_t     = 0x8231, // GL_R8I
  gpu_x_i16_t    = 0x8233, // GL_R16I
  gpu_x_i32_t    = 0x8235, // GL_R32I
  gpu_x_u8_t     = 0x8232, // GL_R8UI
  gpu_x_u16_t    = 0x8234, // GL_R16UI
  gpu_x_u32_t    = 0x8236, // GL_R32UI
  gpu_xy_b8_t    = 0x822B, // GL_RG8
  gpu_xy_f16_t   = 0x822F, // GL_RG16F
  gpu_xy_f32_t   = 0x8230, // GL_RG32F
  gpu_xy_i8_t    = 0x8237, // GL_RG8I
  gpu_xy_i16_t   = 0x8239, // GL_RG16I
  gpu_xy_i32_t   = 0x823B, // GL_RG32I
  gpu_xy_u8_t    = 0x8238, // GL_RG8UI
  gpu_xy_u16_t   = 0x823A, // GL_RG16UI
  gpu_xy_u32_t   = 0x823C, // GL_RG32UI
  gpu_xyz_f32_t  = 0x8815, // GL_RGB32F
  gpu_xyz_i32_t  = 0x8D83, // GL_RGB32I
  gpu_xyz_u32_t  = 0x8D71, // GL_RGB32UI
  gpu_xyzw_b8_t  = 0x8058, // GL_RGBA8
  gpu_xyzw_f16_t = 0x881A, // GL_RGBA16F
  gpu_xyzw_f32_t = 0x8814, // GL_RGBA32F
  gpu_xyzw_i8_t  = 0x8D8E, // GL_RGBA8I
  gpu_xyzw_i16_t = 0x8D88, // GL_RGBA16I
  gpu_xyzw_i32_t = 0x8D82, // GL_RGBA32I
  gpu_xyzw_u8_t  = 0x8D7C, // GL_RGBA8UI
  gpu_xyzw_u16_t = 0x8D76, // GL_RGBA16UI
  gpu_xyzw_u32_t = 0x8D70  // GL_RGBA32UI
};

enum gpu_tex_format_t
{
  gpu_d_f32_t    = 0x8CAC, // GL_DEPTH_COMPONENT32F
  gpu_rgb_b8_t   = 0x8051, // GL_RGB8
  gpu_rgba_b8_t  = 0x8058, // GL_RGBA8
  gpu_srgb_b8_t  = 0x8C41, // GL_SRGB8
  gpu_srgba_b8_t = 0x8C43, // GL_SRGB8_ALPHA8
  gpu_rgba_f32_t = 0x8814  // GL_RGBA32F
};

enum gpu_smp_filter_t
{
  gpu_nearest_t                = 0x2600, // GL_NEAREST
  gpu_linear_t                 = 0x2601, // GL_LINEAR
  gpu_nearest_mipmap_nearest_t = 0x2700, // GL_NEAREST_MIPMAP_NEAREST
  gpu_linear_mipmap_nearest_t  = 0x2701, // GL_LINEAR_MIPMAP_NEAREST
  gpu_nearest_mipmap_linear_t  = 0x2702, // GL_NEAREST_MIPMAP_LINEAR
  gpu_linear_mipmap_linear_t   = 0x2703  // GL_LINEAR_MIPMAP_LINEAR
};

enum gpu_smp_wrapping_t
{
  gpu_repeat_t                 = 0x2901, // GL_REPEAT
  gpu_clamp_to_edge_t          = 0x812F, // GL_CLAMP_TO_EDGE
  gpu_clamp_to_border_t        = 0x812D, // GL_CLAMP_TO_BORDER
  gpu_mirrored_repeat_t        = 0x8370, // GL_MIRRORED_REPEAT
  gpu_mirror_clamp_t           = 0x8742, // GL_MIRROR_CLAMP
  gpu_mirror_clamp_to_edge_t   = 0x8743, // GL_MIRROR_CLAMP_TO_EDGE
  gpu_mirror_clamp_to_border_t = 0x8912  // GL_MIRROR_CLAMP_TO_BORDER
};

enum gpu_pixel_format_t
{
  gpu_rgb_t  = 0x1907, // GL_RGB
  gpu_bgr_t  = 0x80E0, // GL_BGR
  gpu_rgba_t = 0x1908, // GL_RGBA
  gpu_bgra_t = 0x80E1  // GL_BGRA
};

enum gpu_pixel_t
{
  gpu_i8_t  = 0x1400, // GL_BYTE
  gpu_i16_t = 0x1402, // GL_SHORT
  gpu_i32_t = 0x1404, // GL_INT
  gpu_u8_t  = 0x1401, // GL_UNSIGNED_BYTE
  gpu_u16_t = 0x1403, // GL_UNSIGNED_SHORT
  gpu_u32_t = 0x1405, // GL_UNSIGNED_INT
  gpu_f32_t = 0x1406  // GL_FLOAT
};

static inline void gpu_check_exts(int extensions_count, const char ** extensions)
{
  profB(__func__);

  for (int i = 0; i < extensions_count; ++i)
  {
    if (SDL_GL_ExtensionSupported(extensions[i]) == SDL_FALSE)
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR: Unsupported OpenGL Extension", extensions[i], NULL);
  }

  profE(__func__);
}

static inline void gpu_window(
    const char * window_title, int window_width, int window_height, int msaa_samples,
    uint32_t sdl_init_flags, uint32_t sdl_window_flags, SDL_Window ** sdl_window, void ** sdl_glcontext)
{
  profB(__func__);

  int sdl_init = SDL_Init(SDL_INIT_VIDEO | sdl_init_flags);

  SDL_assert(sdl_init == 0);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa_samples);
  SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

  *sdl_window = SDL_CreateWindow(
      window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height,
      SDL_WINDOW_OPENGL | sdl_window_flags);

  SDL_assert(*sdl_window != NULL);

  void * glcontext = SDL_GL_CreateContext(*sdl_window);

  SDL_assert(glcontext != NULL);

  if (sdl_glcontext)
     *sdl_glcontext = glcontext;

  int gl_load = SDL_GL_LoadLibrary(NULL);

  SDL_assert(gl_load == 0);

  const char * extensions[] =
  {
#ifndef RELEASE
      "GL_KHR_debug",
#endif
      "GL_ARB_multi_bind",
      "GL_ARB_gpu_shader5",
      "GL_ARB_clip_control",
      "GL_ARB_texture_view",
      "GL_ARB_base_instance",
      "GL_ARB_buffer_storage",
      "GL_ARB_texture_storage",
      "GL_ARB_gpu_shader_fp64",
      "GL_ARB_shader_precision",
      "GL_ARB_get_program_binary",
      "GL_ARB_transform_feedback2",
      "GL_ARB_direct_state_access",
      "GL_ARB_texture_buffer_range",
      "GL_EXT_texture_mirror_clamp",
      "GL_ARB_get_texture_sub_image",
      "GL_ARB_texture_cube_map_array",
      "GL_ARB_separate_shader_objects",
      "GL_ARB_shading_language_420pack",
      "GL_ARB_shading_language_packing",
      "GL_EXT_texture_filter_anisotropic",
      "GL_ARB_texture_buffer_object_rgb32"
  };

  gpu_check_exts(sizeof(extensions) / sizeof(extensions[0]), extensions);

  glAttachShader = SDL_GL_GetProcAddress("glAttachShader");
  glBeginTransformFeedback = SDL_GL_GetProcAddress("glBeginTransformFeedback");
  glBindFramebuffer = SDL_GL_GetProcAddress("glBindFramebuffer");
  glBindProgramPipeline = SDL_GL_GetProcAddress("glBindProgramPipeline");
  glBindSamplers = SDL_GL_GetProcAddress("glBindSamplers");
  glBindTextures = SDL_GL_GetProcAddress("glBindTextures");
  glBindTransformFeedback = SDL_GL_GetProcAddress("glBindTransformFeedback");
  glBlitNamedFramebuffer = SDL_GL_GetProcAddress("glBlitNamedFramebuffer");
  glClear = SDL_GL_GetProcAddress("glClear");
  glClearColor = SDL_GL_GetProcAddress("glClearColor");
  glCompileShader = SDL_GL_GetProcAddress("glCompileShader");
  glCreateBuffers = SDL_GL_GetProcAddress("glCreateBuffers");
  glCreateFramebuffers = SDL_GL_GetProcAddress("glCreateFramebuffers");
  glCreateProgram = SDL_GL_GetProcAddress("glCreateProgram");
  glCreateProgramPipelines = SDL_GL_GetProcAddress("glCreateProgramPipelines");
  glCreateSamplers = SDL_GL_GetProcAddress("glCreateSamplers");
  glCreateShader = SDL_GL_GetProcAddress("glCreateShader");
  glCreateTextures = SDL_GL_GetProcAddress("glCreateTextures");
  glCreateTransformFeedbacks = SDL_GL_GetProcAddress("glCreateTransformFeedbacks");
  glDebugMessageCallback = SDL_GL_GetProcAddress("glDebugMessageCallback");
  glDeleteBuffers = SDL_GL_GetProcAddress("glDeleteBuffers");
  glDeleteFramebuffers = SDL_GL_GetProcAddress("glDeleteFramebuffers");
  glDeleteProgram = SDL_GL_GetProcAddress("glDeleteProgram");
  glDeleteProgramPipelines = SDL_GL_GetProcAddress("glDeleteProgramPipelines");
  glDeleteSamplers = SDL_GL_GetProcAddress("glDeleteSamplers");
  glDeleteShader = SDL_GL_GetProcAddress("glDeleteShader");
  glDeleteTextures = SDL_GL_GetProcAddress("glDeleteTextures");
  glDeleteTransformFeedbacks = SDL_GL_GetProcAddress("glDeleteTransformFeedbacks");
  glDetachShader = SDL_GL_GetProcAddress("glDetachShader");
  glDisable = SDL_GL_GetProcAddress("glDisable");
  glDrawArraysInstancedBaseInstance = SDL_GL_GetProcAddress("glDrawArraysInstancedBaseInstance");
  glEnable = SDL_GL_GetProcAddress("glEnable");
  glEndTransformFeedback = SDL_GL_GetProcAddress("glEndTransformFeedback");
  glFinish = SDL_GL_GetProcAddress("glFinish");
  glGenerateTextureMipmap = SDL_GL_GetProcAddress("glGenerateTextureMipmap");
  glGenTextures = SDL_GL_GetProcAddress("glGenTextures");
  glGetTextureSubImage = SDL_GL_GetProcAddress("glGetTextureSubImage");
  glLinkProgram = SDL_GL_GetProcAddress("glLinkProgram");
  glMapNamedBufferRange = SDL_GL_GetProcAddress("glMapNamedBufferRange");
  glNamedBufferStorage = SDL_GL_GetProcAddress("glNamedBufferStorage");
  glNamedFramebufferDrawBuffer = SDL_GL_GetProcAddress("glNamedFramebufferDrawBuffer");
  glNamedFramebufferDrawBuffers = SDL_GL_GetProcAddress("glNamedFramebufferDrawBuffers");
  glNamedFramebufferReadBuffer = SDL_GL_GetProcAddress("glNamedFramebufferReadBuffer");
  glNamedFramebufferTextureLayer = SDL_GL_GetProcAddress("glNamedFramebufferTextureLayer");
  glProgramParameteri = SDL_GL_GetProcAddress("glProgramParameteri");
  glProgramUniform1dv = SDL_GL_GetProcAddress("glProgramUniform1dv");
  glProgramUniform1fv = SDL_GL_GetProcAddress("glProgramUniform1fv");
  glProgramUniform1iv = SDL_GL_GetProcAddress("glProgramUniform1iv");
  glProgramUniform1uiv = SDL_GL_GetProcAddress("glProgramUniform1uiv");
  glProgramUniform2fv = SDL_GL_GetProcAddress("glProgramUniform2fv");
  glProgramUniform3fv = SDL_GL_GetProcAddress("glProgramUniform3fv");
  glProgramUniform4fv = SDL_GL_GetProcAddress("glProgramUniform4fv");
  glSamplerParameteri = SDL_GL_GetProcAddress("glSamplerParameteri");
  glScissor = SDL_GL_GetProcAddress("glScissor");
  glShaderSource = SDL_GL_GetProcAddress("glShaderSource");
  glTextureBufferRange = SDL_GL_GetProcAddress("glTextureBufferRange");
  glTextureStorage3D = SDL_GL_GetProcAddress("glTextureStorage3D");
  glTextureStorage3DMultisample = SDL_GL_GetProcAddress("glTextureStorage3DMultisample");
  glTextureSubImage3D = SDL_GL_GetProcAddress("glTextureSubImage3D");
  glTextureView = SDL_GL_GetProcAddress("glTextureView");
  glTransformFeedbackBufferRange = SDL_GL_GetProcAddress("glTransformFeedbackBufferRange");
  glTransformFeedbackVaryings = SDL_GL_GetProcAddress("glTransformFeedbackVaryings");
  glUseProgramStages = SDL_GL_GetProcAddress("glUseProgramStages");
  glViewport = SDL_GL_GetProcAddress("glViewport");

#ifndef RELEASE
  glEnable(0x92E0); // GL_DEBUG_OUTPUT
  glEnable(0x8242); // GL_DEBUG_OUTPUT_SYNCHRONOUS
#endif
  glEnable(0x884F); // GL_TEXTURE_CUBE_MAP_SEAMLESS
  glEnable(0x8DB9); // GL_FRAMEBUFFER_SRGB
  glEnable(0x809D); // GL_MULTISAMPLE
  glEnable(0x864f); // GL_DEPTH_CLAMP
  glEnable(0x0B71); // GL_DEPTH_TEST
  glEnable(0x0B44); // GL_CULL_FACE
  glEnable(0x0BE2); // GL_BLEND

  void (*glBlendFunc)(uint32_t, uint32_t) = SDL_GL_GetProcAddress("glBlendFunc");
  void (*glClipControl)(uint32_t, uint32_t) = SDL_GL_GetProcAddress("glClipControl");
  void (*glDepthRange)(double, double) = SDL_GL_GetProcAddress("glDepthRange");
  void (*glClearDepth)(double) = SDL_GL_GetProcAddress("glClearDepth");
  void (*glDepthFunc)(uint32_t) = SDL_GL_GetProcAddress("glDepthFunc");

  glBlendFunc(0x0302, 0x0303); // GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
  glClipControl(0x8CA1, 0x935F); // GL_LOWER_LEFT, GL_ZERO_TO_ONE
  glDepthRange(1.0, 0.0);
  glClearDepth(0.0);
  glDepthFunc(0x0204); // GL_GREATER

  // OPENGL GARBAGE ///////////////////////////////////////////////////////////////////////////////
  void (*glCreateVertexArrays)(int, uint32_t *) = SDL_GL_GetProcAddress("glCreateVertexArrays"); //
  void (*glBindVertexArray)(uint32_t) = SDL_GL_GetProcAddress("glBindVertexArray");              //
  uint32_t vao = 0;                                                                              //
  glCreateVertexArrays(1, &vao);                                                                 //
  glBindVertexArray(vao);                                                                        //
  /////////////////////////////////////////////////////////////////////////////////////////////////

  profE(__func__);
}

static inline void * gpu_malloc(int bytes)
{
  profB(__func__);

  uint32_t mem_id = 0;
  glCreateBuffers(1, &mem_id);

  int size = 4 + bytes;

  glNamedBufferStorage(mem_id, size, NULL, 194);
  void * p = glMapNamedBufferRange(mem_id, 0, size, 194);

  if (p == NULL)
    return NULL;

  uint32_t * p_u32 = p;
  p_u32[0] = mem_id;
  p_u32 += 1;

  profE(__func__);

  return (void *)p_u32;
}

static inline uint32_t gpu_cast(void * gpu_mem_ptr, enum gpu_tex_mem_format_t format, int bytes_first, int bytes_count)
{
  profB(__func__);

  uint32_t tex_id = 0;
  glCreateTextures(35882, 1, &tex_id);

  uint32_t mem_id = ((uint32_t *)gpu_mem_ptr)[-1];

  glTextureBufferRange(tex_id, format, mem_id, 4 + bytes_first, bytes_count);

  profE(__func__);

  return tex_id;
}

static inline uint32_t gpu_malloc_tex(bool is_cubemap, enum gpu_tex_format_t format, int width, int height, int layer_count, int mipmap_count)
{
  profB(__func__);

  uint32_t tex_id = 0;
  glCreateTextures(is_cubemap ? 36873 : 35866, 1, &tex_id);

  glTextureStorage3D(tex_id, mipmap_count, format, width, height, layer_count * (is_cubemap ? 6 : 1));

  profE(__func__);

  return tex_id;
}

static inline uint32_t gpu_malloc_msi(enum gpu_tex_format_t format, int width, int height, int layer_count, int msaa_samples)
{
  profB(__func__);

  uint32_t tex_id = 0;
  glCreateTextures(37122, 1, &tex_id);

  glTextureStorage3DMultisample(tex_id, msaa_samples, format, width, height, layer_count, 1);

  profE(__func__);

  return tex_id;
}

#define gpu_malloc_img(format, width, height, layer_count, mipmap_count) gpu_malloc_tex(false, format, width, height, layer_count, mipmap_count)
#define gpu_malloc_cbm(format, width, height, layer_count, mipmap_count) gpu_malloc_tex(true, format, width, height, layer_count, mipmap_count)

static inline uint32_t gpu_cast_tex(
    bool is_cubemap, uint32_t tex_id, enum gpu_tex_format_t format, int layer_first, int layer_count, int mipmap_first, int mipmap_count)
{
  profB(__func__);

  uint32_t new_tex_id = 0;
  glGenTextures(1, &new_tex_id);

  glTextureView(
      new_tex_id, is_cubemap ? 36873 : 35866, tex_id, format, mipmap_first, mipmap_count,
      layer_first * (is_cubemap ? 6 : 1),
      layer_count * (is_cubemap ? 6 : 1));

  profE(__func__);

  return new_tex_id;
}

#define gpu_cast_img(tex_id, format, layer_first, layer_count, mipmap_first, mipmap_count) gpu_cast_tex(false, tex_id, format, layer_first, layer_count, mipmap_first, mipmap_count)
#define gpu_cast_cbm(tex_id, format, layer_first, layer_count, mipmap_first, mipmap_count) gpu_cast_tex(true, tex_id, format, layer_first, layer_count, mipmap_first, mipmap_count)

static inline void gpu_get(
    uint32_t tex_id, int layer, int x, int y, int width, int height, uint32_t pixel_format, uint32_t pixel_type, int pixels_bytes, void * pixels)
{
  profB(__func__);
  glGetTextureSubImage(tex_id, 0, x, y, layer, width, height, 1, pixel_format, pixel_type, pixels_bytes, pixels);
  profE(__func__);
}

static inline void gpu_set(
    uint32_t tex_id, int layer, int x, int y, int width, int height, uint32_t pixel_format, uint32_t pixel_type, void * pixels)
{
  profB(__func__);
  glTextureSubImage3D(tex_id, 0, x, y, layer, width, height, 1, pixel_format, pixel_type, pixels);
  profE(__func__);
}

static inline void gpu_mip(uint32_t tex_id)
{
  profB(__func__);
  glGenerateTextureMipmap(tex_id);
  profE(__func__);
}

static inline void gpu_bmp_img(uint32_t tex_id, int width, int height, int layer_count, const char ** bmp_filepaths)
{
  profB(__func__);

  for (int i = 0; i < layer_count; ++i)
  {
    SDL_Surface * bmp = SDL_LoadBMP(bmp_filepaths[i]);
    if (bmp)
    {
      glTextureSubImage3D(tex_id, 0, 0, 0, i, width, height, 1, gpu_bgr_t, gpu_u8_t, bmp->pixels);
      SDL_FreeSurface(bmp);
    }
  }

  profE(__func__);
}

static inline void gpu_bmp_cbm(
    uint32_t tex_id, int width, int height, int layer_count,
    const char ** pos_x_bmp_filepaths,
    const char ** neg_x_bmp_filepaths,
    const char ** pos_y_bmp_filepaths,
    const char ** neg_y_bmp_filepaths,
    const char ** pos_z_bmp_filepaths,
    const char ** neg_z_bmp_filepaths)
{
  profB(__func__);

  for (int i = 0; i < layer_count; ++i)
  {
    SDL_Surface * pos_x_bmp = SDL_LoadBMP(pos_x_bmp_filepaths[i]);
    SDL_Surface * neg_x_bmp = SDL_LoadBMP(neg_x_bmp_filepaths[i]);
    SDL_Surface * pos_y_bmp = SDL_LoadBMP(pos_y_bmp_filepaths[i]);
    SDL_Surface * neg_y_bmp = SDL_LoadBMP(neg_y_bmp_filepaths[i]);
    SDL_Surface * pos_z_bmp = SDL_LoadBMP(pos_z_bmp_filepaths[i]);
    SDL_Surface * neg_z_bmp = SDL_LoadBMP(neg_z_bmp_filepaths[i]);

    if (pos_x_bmp) glTextureSubImage3D(tex_id, 0, 0, 0, i * 6 + 0, width, height, 1, gpu_bgr_t, gpu_u8_t, pos_x_bmp->pixels);
    if (neg_x_bmp) glTextureSubImage3D(tex_id, 0, 0, 0, i * 6 + 1, width, height, 1, gpu_bgr_t, gpu_u8_t, neg_x_bmp->pixels);
    if (pos_y_bmp) glTextureSubImage3D(tex_id, 0, 0, 0, i * 6 + 2, width, height, 1, gpu_bgr_t, gpu_u8_t, pos_y_bmp->pixels);
    if (neg_y_bmp) glTextureSubImage3D(tex_id, 0, 0, 0, i * 6 + 3, width, height, 1, gpu_bgr_t, gpu_u8_t, neg_y_bmp->pixels);
    if (pos_z_bmp) glTextureSubImage3D(tex_id, 0, 0, 0, i * 6 + 4, width, height, 1, gpu_bgr_t, gpu_u8_t, pos_z_bmp->pixels);
    if (neg_z_bmp) glTextureSubImage3D(tex_id, 0, 0, 0, i * 6 + 5, width, height, 1, gpu_bgr_t, gpu_u8_t, neg_z_bmp->pixels);

    if (pos_x_bmp) SDL_FreeSurface(pos_x_bmp);
    if (neg_x_bmp) SDL_FreeSurface(neg_x_bmp);
    if (pos_y_bmp) SDL_FreeSurface(pos_y_bmp);
    if (neg_y_bmp) SDL_FreeSurface(neg_y_bmp);
    if (pos_z_bmp) SDL_FreeSurface(pos_z_bmp);
    if (neg_z_bmp) SDL_FreeSurface(neg_z_bmp);   
  }

  profE(__func__);
}

static inline uint32_t gpu_smp(
    int max_anisotropy, enum gpu_smp_filter_t min_filter, enum gpu_smp_filter_t mag_filter, enum gpu_smp_wrapping_t wrapping)
{
  profB(__func__);

  uint32_t smp_id = 0;
  glCreateSamplers(1, &smp_id);

  glSamplerParameteri(smp_id, 34046, max_anisotropy);
  glSamplerParameteri(smp_id, 10241, min_filter);
  glSamplerParameteri(smp_id, 10240, mag_filter);
  glSamplerParameteri(smp_id, 10242, wrapping);
  glSamplerParameteri(smp_id, 10243, wrapping);
  glSamplerParameteri(smp_id, 32882, wrapping);

  profE(__func__);

  return smp_id;
}

static inline uint32_t gpu_pro(enum gpu_shader_t shader_type, const char * shader_string, int feedback_count, const char ** feedback_names)
{
  profB(__func__);

  uint32_t shader_id = glCreateShader(shader_type);

  glShaderSource(shader_id, 1, (const char **)&shader_string, NULL);
  glCompileShader(shader_id);

  uint32_t pro_id = glCreateProgram();

  glProgramParameteri(pro_id, 33368, 1);
  glAttachShader(pro_id, shader_id);
  if (feedback_count)
    glTransformFeedbackVaryings(pro_id, feedback_count, feedback_names, 35981);
  glLinkProgram(pro_id);
  glDetachShader(pro_id, shader_id);
  glDeleteShader(shader_id);

  profE(__func__);

  return pro_id;
}

static inline uint32_t gpu_pro_file(
    enum gpu_shader_t shader_type, const char * shader_filepath, int feedback_count, const char ** feedback_names)
{
  profB(__func__);

  SDL_RWops * fd = SDL_RWFromFile(shader_filepath, "rb");

  if (fd == NULL)
    return 0;

  SDL_RWseek(fd, 0, RW_SEEK_END);
  int64_t bytes = SDL_RWtell(fd);
  SDL_RWseek(fd, 0, RW_SEEK_SET);
  char src[bytes + 1];
  src[bytes] = 0;
  SDL_RWread(fd, src, bytes, 1);
  SDL_RWclose(fd);
  const char * shader_string = &src[0];

  profE(__func__);

  return gpu_pro(shader_type, shader_string, feedback_count, feedback_names);
}

#define gpu_vert(shader_string) gpu_pro(gpu_vert_t, shader_string, 0, NULL)
#define gpu_frag(shader_string) gpu_pro(gpu_frag_t, shader_string, 0, NULL)
#define gpu_vert_file(shader_filepath) gpu_pro_file(gpu_vert_t, shader_filepath, 0, NULL)
#define gpu_frag_file(shader_filepath) gpu_pro_file(gpu_frag_t, shader_filepath, 0, NULL)
#define gpu_vert_xfb(shader_string, feedback_count, feedback_names) gpu_pro(gpu_vert_t, shader_string, feedback_count, feedback_names)
#define gpu_frag_xfb(shader_string, feedback_count, feedback_names) gpu_pro(gpu_frag_t, shader_string, feedback_count, feedback_names)
#define gpu_vert_xfb_file(shader_filepath, feedback_count, feedback_names) gpu_pro_file(gpu_vert_t, shader_filepath, feedback_count, feedback_names)
#define gpu_frag_xfb_file(shader_filepath, feedback_count, feedback_names) gpu_pro_file(gpu_frag_t, shader_filepath, feedback_count, feedback_names)

#define gpu_f32(program, location, count, value) glProgramUniform1fv(program, location, count, value)
#define gpu_f64(program, location, count, value) glProgramUniform1dv(program, location, count, value)
#define gpu_v_2(program, location, count, value) glProgramUniform2fv(program, location, count, value)
#define gpu_v_3(program, location, count, value) glProgramUniform3fv(program, location, count, value)
#define gpu_v_4(program, location, count, value) glProgramUniform4fv(program, location, count, value)
#define gpu_i32(program, location, count, value) glProgramUniform1iv(program, location, count, value)
#define gpu_u32(program, location, count, value) glProgramUniform1uiv(program, location, count, value)

#define gpu_vert_head                                       \
  "#version 330                                         \n" \
  "#extension GL_ARB_gpu_shader5               : enable \n" \
  "#extension GL_ARB_gpu_shader_fp64           : enable \n" \
  "#extension GL_ARB_shader_precision          : enable \n" \
  "#extension GL_ARB_texture_cube_map_array    : enable \n" \
  "#extension GL_ARB_separate_shader_objects   : enable \n" \
  "#extension GL_ARB_shading_language_420pack  : enable \n" \
  "#extension GL_ARB_shading_language_packing  : enable \n" \
  "#extension GL_ARB_explicit_uniform_location : enable \n" \
  "out gl_PerVertex { vec4 gl_Position; };              \n"

#define gpu_frag_head                                       \
  "#version 330                                         \n" \
  "#extension GL_ARB_gpu_shader5               : enable \n" \
  "#extension GL_ARB_gpu_shader_fp64           : enable \n" \
  "#extension GL_ARB_shader_precision          : enable \n" \
  "#extension GL_ARB_texture_cube_map_array    : enable \n" \
  "#extension GL_ARB_separate_shader_objects   : enable \n" \
  "#extension GL_ARB_shading_language_420pack  : enable \n" \
  "#extension GL_ARB_shading_language_packing  : enable \n" \
  "#extension GL_ARB_explicit_uniform_location : enable \n"

static inline uint32_t gpu_ppo(uint32_t vert_pro_id, uint32_t frag_pro_id)
{
  profB(__func__);

  uint32_t ppo_id = 0;
  glCreateProgramPipelines(1, &ppo_id);

  if (vert_pro_id) glUseProgramStages(ppo_id, 1, vert_pro_id);
  if (frag_pro_id) glUseProgramStages(ppo_id, 2, frag_pro_id);

  profE(__func__);

  return ppo_id;
}

static inline uint32_t gpu_fbo(
    uint32_t color_tex_id_0, int color_tex_layer_0,
    uint32_t color_tex_id_1, int color_tex_layer_1,
    uint32_t color_tex_id_2, int color_tex_layer_2,
    uint32_t color_tex_id_3, int color_tex_layer_3,
    uint32_t depth_tex_id_0, int depth_tex_layer_0)
{
  profB(__func__);

  uint32_t fbo_id = 0;
  glCreateFramebuffers(1, &fbo_id);

  glNamedFramebufferTextureLayer(fbo_id, 36096 + 0, depth_tex_id_0, 0, depth_tex_layer_0);
  glNamedFramebufferTextureLayer(fbo_id, 36064 + 0, color_tex_id_0, 0, color_tex_layer_0);
  glNamedFramebufferTextureLayer(fbo_id, 36064 + 1, color_tex_id_1, 0, color_tex_layer_1);
  glNamedFramebufferTextureLayer(fbo_id, 36064 + 2, color_tex_id_2, 0, color_tex_layer_2);
  glNamedFramebufferTextureLayer(fbo_id, 36064 + 3, color_tex_id_3, 0, color_tex_layer_3);

  int attachments[4];

  attachments[0] = color_tex_id_0 ? 36064 + 0 : 0;
  attachments[1] = color_tex_id_1 ? 36064 + 1 : 0;
  attachments[2] = color_tex_id_2 ? 36064 + 2 : 0;
  attachments[3] = color_tex_id_3 ? 36064 + 3 : 0;

  glNamedFramebufferDrawBuffers(fbo_id, 4, attachments);

  profE(__func__);

  return fbo_id;
}

static inline uint32_t gpu_xfb(
    void * gpu_mem_ptr_0, int mem_0_bytes_first, int mem_0_bytes_count,
    void * gpu_mem_ptr_1, int mem_1_bytes_first, int mem_1_bytes_count,
    void * gpu_mem_ptr_2, int mem_2_bytes_first, int mem_2_bytes_count,
    void * gpu_mem_ptr_3, int mem_3_bytes_first, int mem_3_bytes_count)
{
  profB(__func__);

  uint32_t xfb_id = 0;
  glCreateTransformFeedbacks(1, &xfb_id);

  uint32_t mem_0_id = gpu_mem_ptr_0 ? ((uint32_t *)gpu_mem_ptr_0)[-1] : 0;
  uint32_t mem_1_id = gpu_mem_ptr_1 ? ((uint32_t *)gpu_mem_ptr_1)[-1] : 0;
  uint32_t mem_2_id = gpu_mem_ptr_2 ? ((uint32_t *)gpu_mem_ptr_2)[-1] : 0;
  uint32_t mem_3_id = gpu_mem_ptr_3 ? ((uint32_t *)gpu_mem_ptr_3)[-1] : 0;

  if (mem_0_id) glTransformFeedbackBufferRange(xfb_id, 0, mem_0_id, 4 + mem_0_bytes_first, mem_0_bytes_count);
  if (mem_1_id) glTransformFeedbackBufferRange(xfb_id, 1, mem_1_id, 4 + mem_1_bytes_first, mem_1_bytes_count);
  if (mem_2_id) glTransformFeedbackBufferRange(xfb_id, 2, mem_2_id, 4 + mem_2_bytes_first, mem_2_bytes_count);
  if (mem_3_id) glTransformFeedbackBufferRange(xfb_id, 3, mem_3_id, 4 + mem_3_bytes_first, mem_3_bytes_count);

  profE(__func__);

  return xfb_id;
}

static inline void gpu_bind_fbo(uint32_t fbo_id)
{
  profB(__func__);
  glBindFramebuffer(36160, fbo_id);
  profE(__func__);
}

static inline void gpu_bind_xfb(uint32_t xfb_id)
{
  profB(__func__);
  glBindTransformFeedback(36386, xfb_id);
  profE(__func__);
}

static inline void gpu_draw(int gpu_op_count, const struct gpu_op_t * gpu_op)
{
  profB(__func__);

  for (int i = 0; i < gpu_op_count; ++i)
  {
    struct gpu_op_t op = gpu_op[i];

    if (op.cmd == NULL) continue;

    profB("bind");
    if (op.tex != NULL) glBindTextures(op.tex_first, op.tex_count, op.tex);
    if (op.smp != NULL) glBindSamplers(op.smp_first, op.smp_count, op.smp);

    if (op.vert != 0) glProgramUniform1iv(op.vert, 0, 1, &op.id);
    if (op.frag != 0) glProgramUniform1iv(op.frag, 0, 1, &op.id);

    glBindProgramPipeline(op.ppo);
    profE("bind");

    profB("submit");
    for (int i = 0; i < op.cmd_count; ++i)
    {
      struct gpu_cmd_t cmd = op.cmd[i];
      glDrawArraysInstancedBaseInstance(op.mode, cmd.first, cmd.count, cmd.instance_count, cmd.instance_first);
    }
    profE("submit");
  }

  profE(__func__);
}

static inline void gpu_draw_xfb(int gpu_op_count, const struct gpu_op_t * gpu_op)
{
  profB(__func__);

  for (int i = 0; i < gpu_op_count; ++i)
  {
    struct gpu_op_t op = gpu_op[i];

    if (op.cmd == NULL) continue;

    profB("bind");
    if (op.tex != NULL) glBindTextures(op.tex_first, op.tex_count, op.tex);
    if (op.smp != NULL) glBindSamplers(op.smp_first, op.smp_count, op.smp);

    if (op.vert != 0) glProgramUniform1iv(op.vert, 0, 1, &op.id);
    if (op.frag != 0) glProgramUniform1iv(op.frag, 0, 1, &op.id);

    glBindProgramPipeline(op.ppo);
    profE("bind");

    profB("submit");
    glBeginTransformFeedback(op.mode);
    for (int i = 0; i < op.cmd_count; ++i)
    {
      struct gpu_cmd_t cmd = op.cmd[i];
      glDrawArraysInstancedBaseInstance(op.mode, cmd.first, cmd.count, cmd.instance_count, cmd.instance_first);
    }
    glEndTransformFeedback();
    profE("submit");
  }

  profE(__func__);
}

static inline void gpu_blit(
    uint32_t source_fbo_id, int source_color_id, int source_x, int source_y, int source_width, int source_height,
    uint32_t target_fbo_id, int target_color_id, int target_x, int target_y, int target_width, int target_height)
{
  profB(__func__);

  glNamedFramebufferReadBuffer(source_fbo_id, 36064 + source_color_id);
  glNamedFramebufferDrawBuffer(target_fbo_id, 36064 + target_color_id);

  glBlitNamedFramebuffer(
      source_fbo_id, target_fbo_id,
      source_x, source_y, source_width, source_height,
      target_x, target_y, target_width, target_height, 16384, 9728);

  profE(__func__);
}

static inline void gpu_blit_to_screen(uint32_t fbo_id, int color_id, int width, int height)
{
  profB(__func__);

  glNamedFramebufferReadBuffer(fbo_id, 36064 + color_id);
  glNamedFramebufferDrawBuffer(0, 1029);
  glBlitNamedFramebuffer(fbo_id, 0, 0, 0, width, height, 0, 0, width, height, 16384, 9728);

  profE(__func__);
}

static inline void gpu_swap(SDL_Window * sdl_window)
{
  profB(__func__);

  SDL_GL_SwapWindow(sdl_window);
  glFinish();

  profE(__func__);
}

static inline void gpu_clear()
{
  profB(__func__);
  glClear(16640); // GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT
  profE(__func__);
}
