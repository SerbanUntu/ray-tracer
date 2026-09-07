add_executable(mandelbrot mandelbrot.cpp)
target_link_libraries(mandelbrot PRIVATE raytracer::mandelbrot raytracer::common)
target_compile_definitions(mandelbrot PRIVATE
        MANDELBROT_DATA_PATH="${CMAKE_SOURCE_DIR}/examples/mandelbrot.json"
)

add_executable(raytracer raytracer.cpp)
target_link_libraries(raytracer PRIVATE raytracer::raytracer)
