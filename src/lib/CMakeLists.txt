add_library(
    render_lib
    "camera.cpp"
    "image.cpp"
    "util/random_utils.cpp"
    "materials/dielectric.cpp"
    "materials/lambertian.cpp"
    "materials/lambertian_texture.cpp"
    "materials/cubemap.cpp"
    "materials/metal.cpp"
    "materials/material.cpp" 
    "shapes/floor.cpp"
    "shapes/sphere.cpp"
)

target_include_directories(render_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}   # so "util/vec3.h" resolves
)

set_property(TARGET render_lib PROPERTY CXX_STANDARD 23)