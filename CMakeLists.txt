cmake_minimum_required(VERSION 4.2)

project("raytracer")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(json googletest)

enable_testing()
include(GoogleTest)

add_subdirectory(common)
add_subdirectory(mandelbrot)
add_subdirectory(raytracer)
add_subdirectory(apps)

add_subdirectory(common/test)
add_subdirectory(mandelbrot/test)
