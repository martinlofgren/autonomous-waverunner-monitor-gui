cmake_minimum_required (VERSION 2.6)
project (WR-gui)

# Dear IMGUI and the main app are the same for all platforms
add_library(imgui STATIC
  ./lib/imgui/imgui.cpp
  ./lib/imgui/imgui_draw.cpp
  ./lib/imgui/imgui_widgets.cpp
  ./lib/imgui/imgui_demo.cpp)

add_executable(WR-gui
  src/main.cpp
  src/control.cpp
  src/map.cpp
  src/steering.cpp)

# Under Linux, we use SFML for managing windows and rendering the content.
if(UNIX AND NOT APPLE)

  add_definitions(-DLINUX)

  add_library(imgui-sfml
    ./lib/imgui-sfml/imgui-SFML.cpp)

  find_package(OpenGL REQUIRED)
  include_directories(${OPENGL_INCLUDE_DIR})
  target_link_libraries(WR-gui ${OPENGL_LIBRARIES})

  target_link_libraries(WR-gui
    imgui
    imgui-sfml
    sfml-graphics
    sfml-window
    sfml-system)

  include_directories(
    ./lib/imgui
    ./lib/imgui-sfml
    ./inc)

endif()

# On Windows, we'll use the native rendering
if(WIN32)

  add_definitions(-DWINDOWS)

endif()
