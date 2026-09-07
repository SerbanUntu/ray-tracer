file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(raytracer-mandelbrot INTERFACE)
add_library(raytracer::mandelbrot ALIAS raytracer-mandelbrot)

target_sources(raytracer-mandelbrot
        INTERFACE
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${HEADERS}
)

target_link_libraries(raytracer-mandelbrot
        INTERFACE
        raytracer::common
        nlohmann_json::nlohmann_json
)
