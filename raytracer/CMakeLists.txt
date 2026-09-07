file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(raytracer-raytracer STATIC ${SOURCES})
add_library(raytracer::raytracer ALIAS raytracer-raytracer)

target_sources(raytracer-raytracer
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${HEADERS}
)

target_link_libraries(raytracer-raytracer
        PUBLIC
        raytracer::common
        nlohmann_json::nlohmann_json
)

target_compile_definitions(raytracer-raytracer
        PUBLIC
        RAYTRACER_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)
