cmake_minimum_required(VERSION 4.2)

project(raytracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)         # GTest::gtest_main
find_package(nlohmann_json REQUIRED) # nlohmann_json::nlohmann_json

enable_testing()
include(GoogleTest)

add_subdirectory(common)
add_subdirectory(mandelbrot)
add_subdirectory(raytracer)
add_subdirectory(apps)

add_subdirectory(common/test)
add_subdirectory(mandelbrot/test)
