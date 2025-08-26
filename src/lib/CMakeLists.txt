add_library(render_lib
    image.cpp
    util/random_utils.cpp
 "camera.h" "util/complex.h")

target_include_directories(render_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}   # so "util/vec3.h" resolves
)

set_property(TARGET render_lib PROPERTY CXX_STANDARD 23)