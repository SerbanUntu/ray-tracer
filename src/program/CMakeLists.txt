# CMakeList.txt : CMake project for ray-tracer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ray-tracer "ray-tracer.cpp")
add_executable (mandelbrot "mandelbrot.cpp")

target_link_libraries(ray-tracer PRIVATE render_lib)
target_link_libraries(mandelbrot PRIVATE render_lib)

add_custom_command(
    TARGET ray-tracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/texture.bmp
            $<TARGET_FILE_DIR:ray-tracer>
)

set_property(TARGET ray-tracer PROPERTY CXX_STANDARD 23)
set_property(TARGET mandelbrot PROPERTY CXX_STANDARD 23)
