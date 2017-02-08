#include "gl_functions.h"
#include "imgui/cimgui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>

struct ImVec2
{
  float x, y;
};

struct ImVec3
{
  float x, y, z;
};

struct ImVec4
{
  float x, y, z, w;
};

struct ImGuiIO
{
  struct ImVec2 DisplaySize;
  float DeltaTime;
  float IniSavingRate;
  const char * IniFilename;
  const char * LogFilename;
  float MouseDoubleClickTime;
  float MouseDoubleClickMaxDist;
  float MouseDragThreshold;
  int32_t KeyMap[19];
  float KeyRepeatDelay;
  float KeyRepeatRate;
  void * UserData;
  struct ImFontAtlas * Fonts;
  float FontGlobalScale;
  bool FontAllowUserScaling;
  struct ImVec2 DisplayFramebufferScale;
  struct ImVec2 DisplayVisibleMin;
  struct ImVec2 DisplayVisibleMax;
  bool OSXBehaviors;
  void (*RenderDrawListsFn)(struct ImDrawData * data);
  const char * (*GetClipboardTextFn)();
  void (*SetClipboardTextFn)(const char * text);
  void * (*MemAllocFn)(size_t sz);
  void (*MemFreeFn)(void * ptr);
  void (*ImeSetInputScreenPosFn)(int32_t x, int32_t y);
  void * ImeWindowHandle;
  struct ImVec2 MousePos;
  bool MouseDown[5];
  float MouseWheel;
  bool MouseDrawCursor;
  bool KeyCtrl;
  bool KeyShift;
  bool KeyAlt;
  bool KeySuper;
  bool KeysDown[512];
  uint16_t InputCharacters[17];
  bool WantCaptureMouse;
  bool WantCaptureKeyboard;
  bool WantTextInput;
  float Framerate;
  int32_t MetricsAllocs;
  int32_t MetricsRenderVertices;
  int32_t MetricsRenderIndices;
  int32_t MetricsActiveWindows;
  struct ImVec2 MousePosPrev;
  struct ImVec2 MouseDelta;
  bool MouseClicked[5];
  struct ImVec2 MouseClickedPos[5];
  float MouseClickedTime[5];
  bool MouseDoubleClicked[5];
  bool MouseReleased[5];
  bool MouseDownOwned[5];
  float MouseDownDuration[5];
  float MouseDownDurationPrev[5];
  float MouseDragMaxDistanceSqr[5];
  float KeysDownDuration[512];
  float KeysDownDurationPrev[512];
};

struct ImDrawData
{
  bool Valid;
  struct ImDrawList ** CmdLists;
  int32_t CmdListsCount;
  int32_t TotalVtxCount;
  int32_t TotalIdxCount;
};

struct ImDrawCmd
{
  int32_t ElemCount;
  struct ImVec4 ClipRect;
  ImTextureID TextureId;
  ImDrawCallback UserCallback;
  void * UserCallbackData;
};

typedef struct
{
  struct ImVec2 pos;
  struct ImVec2 uv;
  uint32_t col;
} ImDrawVtx;

enum ImGuiKey
{
  ImGuiKey_Tab,
  ImGuiKey_LeftArrow,
  ImGuiKey_RightArrow,
  ImGuiKey_UpArrow,
  ImGuiKey_DownArrow,
  ImGuiKey_PageUp,
  ImGuiKey_PageDown,
  ImGuiKey_Home,
  ImGuiKey_End,
  ImGuiKey_Delete,
  ImGuiKey_Backspace,
  ImGuiKey_Enter,
  ImGuiKey_Escape,
  ImGuiKey_A,
  ImGuiKey_C,
  ImGuiKey_V,
  ImGuiKey_X,
  ImGuiKey_Y,
  ImGuiKey_Z,
  ImGuiKey_COUNT
};

enum ImGuiCol
{
  ImGuiCol_Text,
  ImGuiCol_TextDisabled,
  ImGuiCol_WindowBg,
  ImGuiCol_ChildWindowBg,
  ImGuiCol_PopupBg,
  ImGuiCol_Border,
  ImGuiCol_BorderShadow,
  ImGuiCol_FrameBg,
  ImGuiCol_FrameBgHovered,
  ImGuiCol_FrameBgActive,
  ImGuiCol_TitleBg,
  ImGuiCol_TitleBgCollapsed,
  ImGuiCol_TitleBgActive,
  ImGuiCol_MenuBarBg,
  ImGuiCol_ScrollbarBg,
  ImGuiCol_ScrollbarGrab,
  ImGuiCol_ScrollbarGrabHovered,
  ImGuiCol_ScrollbarGrabActive,
  ImGuiCol_ComboBg,
  ImGuiCol_CheckMark,
  ImGuiCol_SliderGrab,
  ImGuiCol_SliderGrabActive,
  ImGuiCol_Button,
  ImGuiCol_ButtonHovered,
  ImGuiCol_ButtonActive,
  ImGuiCol_Header,
  ImGuiCol_HeaderHovered,
  ImGuiCol_HeaderActive,
  ImGuiCol_Column,
  ImGuiCol_ColumnHovered,
  ImGuiCol_ColumnActive,
  ImGuiCol_ResizeGrip,
  ImGuiCol_ResizeGripHovered,
  ImGuiCol_ResizeGripActive,
  ImGuiCol_CloseButton,
  ImGuiCol_CloseButtonHovered,
  ImGuiCol_CloseButtonActive,
  ImGuiCol_PlotLines,
  ImGuiCol_PlotLinesHovered,
  ImGuiCol_PlotHistogram,
  ImGuiCol_PlotHistogramHovered,
  ImGuiCol_TextSelectedBg,
  ImGuiCol_ModalWindowDarkening,
  ImGuiCol_COUNT
};

struct ImGuiStyle
{
  float Alpha;
  struct ImVec2 WindowPadding;
  struct ImVec2 WindowMinSize;
  float WindowRounding;
  struct ImVec2 WindowTitleAlign;
  float ChildWindowRounding;
  struct ImVec2 FramePadding;
  float FrameRounding;
  struct ImVec2 ItemSpacing;
  struct ImVec2 ItemInnerSpacing;
  struct ImVec2 TouchExtraPadding;
  float IndentSpacing;
  float ColumnsMinSpacing;
  float ScrollbarSize;
  float ScrollbarRounding;
  float GrabMinSize;
  float GrabRounding;
  struct ImVec2 ButtonTextAlign;
  struct ImVec2 DisplayWindowPadding;
  struct ImVec2 DisplaySafeAreaPadding;
  bool AntiAliasedLines;
  bool AntiAliasedShapes;
  float CurveTessellationTol;
  struct ImVec4 Colors[43];
};

static double g_time = 0.0;
static float g_mouse_wheel = 0.f;
static bool g_is_mouse_pressed[3] = {};
static uint32_t g_font_texture = 0, g_ppo = 0, g_vert = 0, g_frag = 0;

static void imgui_render_draw_list(struct ImDrawData * draw_data)
{
  struct ImGuiIO * io = igGetIO();

  struct ImVec2 fb_scale = io->DisplayFramebufferScale;
  int32_t w = (int32_t)(io->DisplaySize.x * fb_scale.x);
  int32_t h = (int32_t)(io->DisplaySize.y * fb_scale.y);

  ImDrawData_ScaleClipRects(draw_data, fb_scale);
  glViewport(0, 0, w, h);

  glDisable(0x0B44); // GL_CULL_FACE
  glDisable(0x0B71); // GL_DEPTH_TEST
  glEnable(0x0C11);  // GL_SCISSOR_TEST
  glDisable(0x8DB9); // GL_FRAMEBUFFER_SRGB

  float scale[2], translate[2];
  scale[0] = 2.f / w;
  scale[1] = 2.f / -h;
  translate[0] = -1.f;
  translate[1] = 1.f;
  glProgramUniform2fv(g_vert, 0, 1, scale);
  glProgramUniform2fv(g_vert, 1, 1, translate);
  glBindProgramPipeline(g_ppo);

  ptrdiff_t idx_bytes = draw_data->TotalIdxCount * (ptrdiff_t)sizeof(ImDrawIdx);
  ptrdiff_t vtx_bytes = draw_data->TotalVtxCount * (ptrdiff_t)sizeof(ImDrawVtx);

  uint32_t idx_mem_id = 0;
  uint32_t vtx_mem_id = 0;
  glCreateBuffers(1, &idx_mem_id);
  glCreateBuffers(1, &vtx_mem_id);
  glNamedBufferStorage(idx_mem_id, idx_bytes, 0, 194);
  glNamedBufferStorage(vtx_mem_id, vtx_bytes, 0, 194);
  void * idx = glMapNamedBufferRange(idx_mem_id, 0, idx_bytes, 194);
  void * vtx = glMapNamedBufferRange(vtx_mem_id, 0, vtx_bytes, 194);
  uint32_t idx_tex_id = 0;
  uint32_t vtx_f32_tex_id = 0;
  uint32_t vtx_u32_tex_id = 0;
  glCreateTextures(35882, 1, &idx_tex_id);
  glCreateTextures(35882, 1, &vtx_f32_tex_id);
  glCreateTextures(35882, 1, &vtx_u32_tex_id);
  glTextureBufferRange(idx_tex_id, 0x8234, idx_mem_id, 0, idx_bytes); // GL_R16UI
  glTextureBufferRange(vtx_f32_tex_id, 0x822E, vtx_mem_id, 0, vtx_bytes); // GL_R32F
  glTextureBufferRange(vtx_u32_tex_id, 0x8236, vtx_mem_id, 0, vtx_bytes); // GL_R32UI

  ImDrawIdx * idx_dest = idx;
  ImDrawVtx * vtx_dest = vtx;
  for (ptrdiff_t i = 0, c = draw_data->CmdListsCount; i < c; ++i)
  {
    struct ImDrawList * cmd_list = draw_data->CmdLists[i];
    int32_t idx_size = ImDrawList_GetIndexBufferSize(cmd_list);
    int32_t vtx_size = ImDrawList_GetVertexBufferSize(cmd_list);
    SDL_memcpy(idx_dest, ImDrawList_GetIndexPtr(cmd_list, 0), (size_t)idx_size * sizeof(ImDrawIdx));
    SDL_memcpy(vtx_dest, ImDrawList_GetVertexPtr(cmd_list, 0), (size_t)vtx_size * sizeof(ImDrawVtx));
    idx_dest += idx_size;
    vtx_dest += vtx_size;
  }

  uint32_t input[4];
  input[0] = 0;
  input[1] = vtx_f32_tex_id;
  input[2] = vtx_u32_tex_id;
  input[3] = idx_tex_id;

  int32_t idx_offset = 0;
  int32_t vtx_offset = 0;
  for (ptrdiff_t i = 0, c = draw_data->CmdListsCount; i < c; ++i)
  {
    struct ImDrawList * cmd_list = draw_data->CmdLists[i];
    for (ptrdiff_t j = 0, c = ImDrawList_GetCmdSize(cmd_list); j < c; ++j)
    {
      struct ImDrawCmd * pcmd = ImDrawList_GetCmdPtr(cmd_list, (int32_t)j);
      if (pcmd->UserCallback)
      {
        pcmd->UserCallback(cmd_list, pcmd);
      }
      else
      {
        glScissor(
            (int32_t)(pcmd->ClipRect.x), (int32_t)(h - pcmd->ClipRect.w),
            (int32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x),
            (int32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y));
        glProgramUniform1iv(g_vert, 2, 1, &vtx_offset);
        input[0] = *(uint32_t *)pcmd->TextureId;
        glBindTextures(0, 4, input);
        glDrawArraysInstancedBaseInstance(0x0004, idx_offset, pcmd->ElemCount, 1, 0); // GL_TRIANGLES
      }
      idx_offset += pcmd->ElemCount;
    }
    vtx_offset += ImDrawList_GetVertexBufferSize(cmd_list);
  }

  glDeleteBuffers(1, &idx_mem_id);
  glDeleteBuffers(1, &vtx_mem_id);
  glDeleteTextures(1, &idx_tex_id);
  glDeleteTextures(1, &vtx_f32_tex_id);
  glDeleteTextures(1, &vtx_u32_tex_id);

  glEnable(0x8DB9);  // GL_FRAMEBUFFER_SRGB
  glDisable(0x0C11); // GL_SCISSOR_TEST
  glEnable(0x0B71);  // GL_DEPTH_TEST
  glEnable(0x0B44);  // GL_CULL_FACE
}

static const char * imgui_get_clipboard_text()
{
  return SDL_GetClipboardText();
}

static void imgui_set_clipboard_text(const char * text)
{
  SDL_SetClipboardText(text);
}

bool imgui_process_event(SDL_Event * event)
{
  struct ImGuiIO * io = igGetIO();
  switch (event->type)
  {
    case (SDL_MOUSEWHEEL):
      if (event->wheel.y > 0)
        g_mouse_wheel = 1;
      if (event->wheel.y < 0)
        g_mouse_wheel = -1;
      return true;
    case (SDL_MOUSEBUTTONDOWN):
      if (event->button.button == SDL_BUTTON_LEFT)
        g_is_mouse_pressed[0] = true;
      if (event->button.button == SDL_BUTTON_RIGHT)
        g_is_mouse_pressed[1] = true;
      if (event->button.button == SDL_BUTTON_MIDDLE)
        g_is_mouse_pressed[2] = true;
      return true;
    case (SDL_TEXTINPUT):
      ImGuiIO_AddInputCharactersUTF8(event->text.text);
      return true;
    case (SDL_KEYDOWN):
    {
      int32_t key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
      io->KeysDown[key] = (event->type == SDL_KEYDOWN);
      io->KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
      io->KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
      io->KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
      io->KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
      return true;
    }
    case (SDL_KEYUP):
    {
      int32_t key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
      io->KeysDown[key] = (event->type == SDL_KEYDOWN);
      io->KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
      io->KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
      io->KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
      io->KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
      return true;
    }
  }
  return false;
}

void imgui_create_font_texture()
{
  struct ImGuiIO * io = igGetIO();
  uint8_t * pixels;
  int32_t width, height, bpp;
  ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &pixels, &width, &height, &bpp);
  glCreateTextures(35866, 1, &g_font_texture);
  glTextureStorage3D(g_font_texture, 1, 0x8814, width, height, 1); // GL_RGBA32F
  glTextureSubImage3D(g_font_texture, 0, 0, 0, 0, width, height, 1, 0x1908, 0x1401, pixels); // GL_RGBA, GL_UNSIGNED_BYTE
  ImFontAtlas_SetTexID(io->Fonts, &g_font_texture);
}

void imgui_create_device_objects()
{
  const char * vert_string =
      "#version 330                                                          \n"
      "#extension GL_ARB_gpu_shader5               : enable                  \n"
      "#extension GL_ARB_gpu_shader_fp64           : enable                  \n"
      "#extension GL_ARB_shader_precision          : enable                  \n"
      "#extension GL_ARB_texture_cube_map_array    : enable                  \n"
      "#extension GL_ARB_separate_shader_objects   : enable                  \n"
      "#extension GL_ARB_shading_language_420pack  : enable                  \n"
      "#extension GL_ARB_shading_language_packing  : enable                  \n"
      "#extension GL_ARB_explicit_uniform_location : enable                  \n"
      "                                                                      \n"
      " out gl_PerVertex { vec4 gl_Position; };                              \n"
      "                                                                      \n"
      " layout(location = 0) uniform vec2 scale;                             \n"
      " layout(location = 1) uniform vec2 translate;                         \n"
      " layout(location = 2) uniform int vtx_offset;                         \n"
      "                                                                      \n"
      " layout(binding = 1) uniform  samplerBuffer s_vtx_f32;                \n"
      " layout(binding = 2) uniform usamplerBuffer s_vtx_u32;                \n"
      " layout(binding = 3) uniform isamplerBuffer s_idx;                    \n"
      "                                                                      \n"
      " layout(location = 0) out vec2 fs_uv;                                 \n"
      " layout(location = 1) out vec4 fs_col;                                \n"
      "                                                                      \n"
      " void main()                                                          \n"
      " {                                                                    \n"
      "   int i = texelFetch(s_idx, gl_VertexID).x;                          \n"
      "   float posx = texelFetch(s_vtx_f32, (vtx_offset + i) * 5 + 0).x;    \n"
      "   float posy = texelFetch(s_vtx_f32, (vtx_offset + i) * 5 + 1).x;    \n"
      "   float uvx  = texelFetch(s_vtx_f32, (vtx_offset + i) * 5 + 2).x;    \n"
      "   float uvy  = texelFetch(s_vtx_f32, (vtx_offset + i) * 5 + 3).x;    \n"
      "   uint  col  = texelFetch(s_vtx_u32, (vtx_offset + i) * 5 + 4).x;    \n"
      "   fs_uv = vec2(uvx, uvy);                                            \n"
      "   fs_col = unpackUnorm4x8(col);                                      \n"
      "   gl_Position = vec4(fma(vec2(posx, posy), scale, translate), 0, 1); \n"
      " }                                                                    \n";

  const char * frag_string =
      "#version 330                                               \n"
      "#extension GL_ARB_gpu_shader5               : enable       \n"
      "#extension GL_ARB_gpu_shader_fp64           : enable       \n"
      "#extension GL_ARB_shader_precision          : enable       \n"
      "#extension GL_ARB_texture_cube_map_array    : enable       \n"
      "#extension GL_ARB_separate_shader_objects   : enable       \n"
      "#extension GL_ARB_shading_language_420pack  : enable       \n"
      "#extension GL_ARB_shading_language_packing  : enable       \n"
      "#extension GL_ARB_explicit_uniform_location : enable       \n"
      "                                                           \n"
      " layout(binding = 0) uniform sampler2DArray s_texture;     \n"
      "                                                           \n"
      " layout(location = 0) in vec2 fs_uv;                       \n"
      " layout(location = 1) in vec4 fs_col;                      \n"
      "                                                           \n"
      " out vec4 color;                                           \n"
      "                                                           \n"
      " void main()                                               \n"
      " {                                                         \n"
      "   color = fs_col * texture(s_texture, vec3(fs_uv.st, 0)); \n"
      " }                                                         \n";

  uint32_t vert_shader_id = glCreateShader(0x8B31); // GL_VERTEX_SHADER
  uint32_t frag_shader_id = glCreateShader(0x8B30); // GL_FRAGMENT_SHADER

  glShaderSource(vert_shader_id, 1, (const char **)&vert_string, NULL);
  glShaderSource(frag_shader_id, 1, (const char **)&frag_string, NULL);

  glCompileShader(vert_shader_id);
  glCompileShader(frag_shader_id);

  g_vert = glCreateProgram();
  g_frag = glCreateProgram();

  glProgramParameteri(g_vert, 0x8258, 1); // GL_PROGRAM_SEPARABLE
  glProgramParameteri(g_frag, 0x8258, 1); // GL_PROGRAM_SEPARABLE

  glAttachShader(g_vert, vert_shader_id);
  glAttachShader(g_frag, frag_shader_id);

  glLinkProgram(g_vert);
  glLinkProgram(g_frag);

  glDetachShader(g_vert, vert_shader_id);
  glDetachShader(g_frag, frag_shader_id);

  glDeleteShader(vert_shader_id);
  glDeleteShader(frag_shader_id);

  glCreateProgramPipelines(1, &g_ppo);

  glUseProgramStages(g_ppo, 0x00000001, g_vert); // GL_VERTEX_SHADER_BIT
  glUseProgramStages(g_ppo, 0x00000002, g_frag); // GL_FRAGMENT_SHADER_BIT

  imgui_create_font_texture();
}

void imgui_new_frame(SDL_Window * sdl_window)
{
  struct ImGuiIO * io = igGetIO();

  if (!g_font_texture)
    imgui_create_device_objects();

  int32_t w, h, dp_w, dp_h;
  SDL_GetWindowSize(sdl_window, &w, &h);
  SDL_GL_GetDrawableSize(sdl_window, &dp_w, &dp_h);
  io->DisplaySize = (struct ImVec2){(float)w,(float)h};
  io->DisplayFramebufferScale = (struct ImVec2)
  {
    w > 0 ? ((float)dp_w / w) : 0,
    h > 0 ? ((float)dp_h / h) : 0
  };

  uint32_t time = SDL_GetTicks();
  double current_time = time / 1000.0;
  io->DeltaTime = g_time > 0.0 ? (float)(current_time - g_time) : 1.f / 60.f;
  g_time = current_time;

  int32_t mx, my;
  uint32_t mousemask = SDL_GetMouseState(&mx, &my);
  if (SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_MOUSE_FOCUS)
    io->MousePos = (struct ImVec2){(float)mx, (float)my};
  else
    io->MousePos = (struct ImVec2){-1.f, -1.f};

  io->MouseDown[0] = g_is_mouse_pressed[0] || (mousemask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
  io->MouseDown[1] = g_is_mouse_pressed[1] || (mousemask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
  io->MouseDown[2] = g_is_mouse_pressed[2] || (mousemask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
  g_is_mouse_pressed[0] = g_is_mouse_pressed[1] = g_is_mouse_pressed[2] = false;

  io->MouseWheel = g_mouse_wheel;
  g_mouse_wheel = 0.f;

  SDL_ShowCursor(io->MouseDrawCursor ? 0 : 1);

  igNewFrame();
}

static void imgui_invalidate_device_objects()
{
  struct ImGuiIO * io = igGetIO();

  glDeleteProgram(g_vert);
  glDeleteProgram(g_frag);
  glDeleteProgramPipelines(1, &g_ppo);
  g_vert = 0, g_frag = 0, g_ppo = 0;
  if (g_font_texture)
  {
    glDeleteTextures(1, &g_font_texture);
    ImFontAtlas_SetTexID(io->Fonts, 0);
    g_font_texture = 0;
  }
}

static void imgui_deinit()
{
  imgui_invalidate_device_objects();
  igShutdown();
}

static void imgui_init()
{
  struct ImGuiIO * io = igGetIO();

  io->KeyMap[ImGuiKey_Tab] = SDLK_TAB;
  io->KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
  io->KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
  io->KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
  io->KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
  io->KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
  io->KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
  io->KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
  io->KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
  io->KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
  io->KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
  io->KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
  io->KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
  io->KeyMap[ImGuiKey_A] = SDLK_a;
  io->KeyMap[ImGuiKey_C] = SDLK_c;
  io->KeyMap[ImGuiKey_V] = SDLK_v;
  io->KeyMap[ImGuiKey_X] = SDLK_x;
  io->KeyMap[ImGuiKey_Y] = SDLK_y;
  io->KeyMap[ImGuiKey_Z] = SDLK_z;

  io->RenderDrawListsFn = imgui_render_draw_list;
  io->GetClipboardTextFn = imgui_get_clipboard_text;
  io->SetClipboardTextFn = imgui_set_clipboard_text;
}

static void imgui_easy_theming(
    struct ImVec3 color_for_text, struct ImVec3 color_for_head,
    struct ImVec3 color_for_area, struct ImVec3 color_for_body,
    struct ImVec3 color_for_pops)
{
  struct ImGuiStyle * style = igGetStyle();

  style->Colors[ImGuiCol_Text]                 = (struct ImVec4){color_for_text.x, color_for_text.y, color_for_text.z, 1.00f};
  style->Colors[ImGuiCol_TextDisabled]         = (struct ImVec4){color_for_text.x, color_for_text.y, color_for_text.z, 0.58f};
  style->Colors[ImGuiCol_WindowBg]             = (struct ImVec4){color_for_body.x, color_for_body.y, color_for_body.z, 1.00f};
  style->Colors[ImGuiCol_ChildWindowBg]        = (struct ImVec4){color_for_area.x, color_for_area.y, color_for_area.z, 0.58f};
  style->Colors[ImGuiCol_Border]               = (struct ImVec4){color_for_body.x, color_for_body.y, color_for_body.z, 0.00f};
  style->Colors[ImGuiCol_BorderShadow]         = (struct ImVec4){color_for_body.x, color_for_body.y, color_for_body.z, 0.00f};
  style->Colors[ImGuiCol_FrameBg]              = (struct ImVec4){color_for_area.x, color_for_area.y, color_for_area.z, 1.00f};
  style->Colors[ImGuiCol_FrameBgHovered]       = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.78f};
  style->Colors[ImGuiCol_FrameBgActive]        = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_TitleBg]              = (struct ImVec4){color_for_area.x, color_for_area.y, color_for_area.z, 1.00f};
  style->Colors[ImGuiCol_TitleBgCollapsed]     = (struct ImVec4){color_for_area.x, color_for_area.y, color_for_area.z, 0.75f};
  style->Colors[ImGuiCol_TitleBgActive]        = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_MenuBarBg]            = (struct ImVec4){color_for_area.x, color_for_area.y, color_for_area.z, 1.00f};
  style->Colors[ImGuiCol_ScrollbarBg]          = (struct ImVec4){color_for_area.x, color_for_area.y, color_for_area.z, 1.00f};
  style->Colors[ImGuiCol_ScrollbarGrab]        = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.50f};
  style->Colors[ImGuiCol_ScrollbarGrabHovered] = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.78f};
  style->Colors[ImGuiCol_ScrollbarGrabActive]  = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_ComboBg]              = (struct ImVec4){color_for_area.x, color_for_area.y, color_for_area.z, 1.00f};
  style->Colors[ImGuiCol_CheckMark]            = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.80f};
  style->Colors[ImGuiCol_SliderGrab]           = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.50f};
  style->Colors[ImGuiCol_SliderGrabActive]     = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_Button]               = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.50f};
  style->Colors[ImGuiCol_ButtonHovered]        = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.86f};
  style->Colors[ImGuiCol_ButtonActive]         = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_Header]               = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.76f};
  style->Colors[ImGuiCol_HeaderHovered]        = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.86f};
  style->Colors[ImGuiCol_HeaderActive]         = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_Column]               = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.32f};
  style->Colors[ImGuiCol_ColumnHovered]        = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.78f};
  style->Colors[ImGuiCol_ColumnActive]         = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_ResizeGrip]           = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.15f};
  style->Colors[ImGuiCol_ResizeGripHovered]    = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.78f};
  style->Colors[ImGuiCol_ResizeGripActive]     = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_CloseButton]          = (struct ImVec4){color_for_text.x, color_for_text.y, color_for_text.z, 0.16f};
  style->Colors[ImGuiCol_CloseButtonHovered]   = (struct ImVec4){color_for_text.x, color_for_text.y, color_for_text.z, 0.39f};
  style->Colors[ImGuiCol_CloseButtonActive]    = (struct ImVec4){color_for_text.x, color_for_text.y, color_for_text.z, 1.00f};
  style->Colors[ImGuiCol_PlotLines]            = (struct ImVec4){color_for_text.x, color_for_text.y, color_for_text.z, 0.63f};
  style->Colors[ImGuiCol_PlotLinesHovered]     = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_PlotHistogram]        = (struct ImVec4){color_for_text.x, color_for_text.y, color_for_text.z, 0.63f};
  style->Colors[ImGuiCol_PlotHistogramHovered] = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 1.00f};
  style->Colors[ImGuiCol_TextSelectedBg]       = (struct ImVec4){color_for_head.x, color_for_head.y, color_for_head.z, 0.43f};
  style->Colors[ImGuiCol_PopupBg]              = (struct ImVec4){color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f};
  style->Colors[ImGuiCol_ModalWindowDarkening] = (struct ImVec4){color_for_area.x, color_for_area.y, color_for_area.z, 0.73f};
}
