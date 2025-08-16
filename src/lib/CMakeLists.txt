add_library(raytracer_lib
    image.cpp
    util/random_utils.cpp
 "camera.h")

target_include_directories(raytracer_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}   # so "util/vec3.h" resolves
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET raytracer_lib PROPERTY CXX_STANDARD 20)
endif()