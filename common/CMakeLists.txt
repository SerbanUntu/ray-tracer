file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(raytracer-common STATIC ${SOURCES})
add_library(raytracer::common ALIAS raytracer-common)

target_sources(raytracer-common
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${HEADERS}
)

target_link_libraries(raytracer-common
        PUBLIC
        nlohmann_json::nlohmann_json
)
