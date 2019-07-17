cmake_minimum_required (VERSION 2.6)
project (WR-gui)

add_library(imgui STATIC
  ../imgui/imgui.cpp
  ../imgui/imgui_draw.cpp
  ../imgui/imgui_widgets.cpp
  ../imgui/imgui_demo.cpp
  ../imgui-sfml/imgui-SFML.cpp)

add_executable(WR-gui
  ../imgui-sfml/imgui-SFML.cpp
  ../imgui/imgui.cpp
  ../imgui/imgui_draw.cpp
  ../imgui/imgui_widgets.cpp
  ../imgui/imgui_demo.cpp
  src/main.cpp
  src/control.cpp
  src/map.cpp
  src/steering.cpp)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(WR-gui ${OPENGL_LIBRARIES})

target_link_libraries(WR-gui
  sfml-graphics
  sfml-window
  sfml-system
  imgui)

include_directories(
  ../imgui
  ../imgui-sfml
  ./inc)
