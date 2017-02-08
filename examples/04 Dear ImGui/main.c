#include "../../gpulib.h"
#include "../../gpulib_imgui.h"

int32_t main()
{
  SDL_Window * sdl_window = NULL;
  gpu_window("Dear ImGui", 1280, 720, 4, 0, 0, &sdl_window, NULL);

  imgui_init();

  struct ImGuiIO * io = igGetIO();
  struct ImGuiStyle * style = igGetStyle();

  char font[10000] = {};
  SDL_snprintf(font, 10000, "%s%s", SDL_GetBasePath(), "NotoSans.ttf");
  ImFontAtlas_AddFontFromFileTTF(io->Fonts, font, 24.f, NULL, NULL);

  style->ScrollbarRounding = 0;
  style->WindowRounding = 0;
  style->FrameRounding = 0;

  while (1)
  {
    for (SDL_Event e; SDL_PollEvent(&e);)
    {
      imgui_process_event(&e);
      if (e.type == SDL_QUIT)
        goto exit;
    }

    gpu_clear();
    imgui_new_frame(sdl_window);

    {
      // Flat UI by yorick.penninks: https://color.adobe.com/Flat-UI-color-theme-2469224/
      static struct ImVec3 color_for_text = (struct ImVec3){236.f / 255.f, 240.f / 255.f, 241.f / 255.f};
      static struct ImVec3 color_for_head = (struct ImVec3){ 41.f / 255.f, 128.f / 255.f, 185.f / 255.f};
      static struct ImVec3 color_for_area = (struct ImVec3){ 57.f / 255.f,  79.f / 255.f, 105.f / 255.f};
      static struct ImVec3 color_for_body = (struct ImVec3){ 44.f / 255.f,  62.f / 255.f,  80.f / 255.f};
      static struct ImVec3 color_for_pops = (struct ImVec3){ 33.f / 255.f,  46.f / 255.f,  60.f / 255.f};

      // Mint Y Dark:
      //static struct ImVec3 color_for_text = (struct ImVec3){211.f / 255.f, 211.f / 255.f, 211.f / 255.f};
      //static struct ImVec3 color_for_head = (struct ImVec3){ 95.f / 255.f, 142.f / 255.f,  85.f / 255.f};
      //static struct ImVec3 color_for_area = (struct ImVec3){ 47.f / 255.f,  47.f / 255.f,  47.f / 255.f};
      //static struct ImVec3 color_for_body = (struct ImVec3){ 64.f / 255.f,  64.f / 255.f,  64.f / 255.f};
      //static struct ImVec3 color_for_pops = (struct ImVec3){ 30.f / 255.f,  30.f / 255.f,  30.f / 255.f};

      // Arc Theme:
      //static struct ImVec3 color_for_text = (struct ImVec3){211.f / 255.f, 218.f / 255.f, 227.f / 255.f};
      //static struct ImVec3 color_for_head = (struct ImVec3){ 64.f / 255.f, 132.f / 255.f, 214.f / 255.f};
      //static struct ImVec3 color_for_area = (struct ImVec3){ 47.f / 255.f,  52.f / 255.f,  63.f / 255.f};
      //static struct ImVec3 color_for_body = (struct ImVec3){ 56.f / 255.f,  60.f / 255.f,  74.f / 255.f};
      //static struct ImVec3 color_for_pops = (struct ImVec3){ 28.f / 255.f,  30.f / 255.f,  37.f / 255.f};

      igColorEdit3("Text", &color_for_text.x);
      igColorEdit3("Head", &color_for_head.x);
      igColorEdit3("Area", &color_for_area.x);
      igColorEdit3("Body", &color_for_body.x);
      igColorEdit3("Pops", &color_for_pops.x);
      
      imgui_easy_theming(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops);
    }

    static bool show_test_window = true;
    igShowTestWindow(&show_test_window);

    igRender();
    gpu_swap(sdl_window);
  }

exit:
  imgui_deinit();
  return 0;
}
