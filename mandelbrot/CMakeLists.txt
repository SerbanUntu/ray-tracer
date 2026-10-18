file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(raytracer-mandelbrot STATIC ${SOURCES})
add_library(raytracer::mandelbrot ALIAS raytracer-mandelbrot)

target_sources(raytracer-mandelbrot
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${HEADERS}
)

target_link_libraries(raytracer-mandelbrot
        PUBLIC
        raytracer::common
        nlohmann_json::nlohmann_json
)
