#!/bin/bash
cd "$(dirname -- "$(readlink -fn -- "${0}")")"

clang++ -c imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/cimgui.cpp imgui/fontAtlas.cpp imgui/drawList.cpp -static -stdlib=libc++ ${@}
ar -cr libimgui.a cimgui.o drawList.o fontAtlas.o imgui.o imgui_demo.o imgui_draw.o
rm -f cimgui.o drawList.o fontAtlas.o imgui.o imgui_demo.o imgui_draw.o
