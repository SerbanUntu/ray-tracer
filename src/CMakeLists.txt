# CMakeList.txt : CMake project for ray-tracer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ray-tracer "ray-tracer.cpp" "ray-tracer.h"  "image.h" "util/vec3.h"  "sphere.h"  "image.cpp" "object.h" "floor.h"  "materials/material.h" "materials/metal.h" "materials/lambertian.h" "util/random_utils.cpp" "util/random_utils.h" "materials/dielectric.h" "cubemap.h" "materials/lambertian_texture.h")

add_custom_command(
    TARGET ray-tracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/texture.bmp
            $<TARGET_FILE_DIR:ray-tracer>
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ray-tracer PROPERTY CXX_STANDARD 20)
endif()

