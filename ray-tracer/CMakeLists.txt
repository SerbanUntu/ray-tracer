﻿# CMakeList.txt : CMake project for ray-tracer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ray-tracer "ray-tracer.cpp" "ray-tracer.h" "color.h" "image.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ray-tracer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
