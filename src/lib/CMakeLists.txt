add_library(raytracer_lib
    image.cpp
    util/random_utils.cpp
 "camera.h" "util/complex.h")

target_include_directories(raytracer_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}   # so "util/vec3.h" resolves
)

set_property(TARGET raytracer_lib PROPERTY CXX_STANDARD 23)