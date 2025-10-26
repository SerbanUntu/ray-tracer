# CMakeList.txt : CMake project for ray-tracer, include source and define
# project specific logic here.
#

add_executable (ray-tracer "ray-tracer.cpp")
add_executable (mandelbrot "mandelbrot.cpp")

target_link_libraries(ray-tracer PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(ray-tracer PRIVATE render_lib)
target_link_libraries(mandelbrot PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(mandelbrot PRIVATE render_lib)

set(MANDELBROT_DATA_PATH "${CMAKE_SOURCE_DIR}/examples/mandelbrot.json")
set(RAY_TRACER_DATA_PATH "${CMAKE_SOURCE_DIR}/examples/ray-tracer.json")

target_compile_definitions(mandelbrot PRIVATE
    MANDELBROT_DATA_PATH="${MANDELBROT_DATA_PATH}"
)

target_compile_definitions(ray-tracer PRIVATE
    RAY_TRACER_DATA_PATH="${RAY_TRACER_DATA_PATH}"
)

add_custom_command(
    TARGET ray-tracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/texture.bmp
            $<TARGET_FILE_DIR:ray-tracer>
)

set_property(TARGET ray-tracer PROPERTY CXX_STANDARD 23)
set_property(TARGET mandelbrot PROPERTY CXX_STANDARD 23)
