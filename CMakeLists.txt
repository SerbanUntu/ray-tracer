cmake_minimum_required(VERSION 4.2)

project(raytracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(RAYTRACER_FAST_MATH "Compile with -ffast-math" OFF)
if (RAYTRACER_FAST_MATH)
    if (MSVC)
        add_compile_options(/fp:fast)
    else ()
        add_compile_options(-ffast-math)
    endif ()
endif ()

set(RAYTRACER_OPT_LEVEL "" CACHE STRING "Release optimization level (0, 1, 2 or 3)")
set_property(CACHE RAYTRACER_OPT_LEVEL PROPERTY STRINGS "" 0 1 2 3)
if (NOT RAYTRACER_OPT_LEVEL STREQUAL "")
    if (NOT RAYTRACER_OPT_LEVEL MATCHES "^[0-3]$")
        message(FATAL_ERROR "RAYTRACER_OPT_LEVEL must be 0, 1, 2 or 3, got '${RAYTRACER_OPT_LEVEL}'")
    endif ()
    if (MSVC)
        set(_msvc_opt_flags "/Od" "/O1" "/O2 /Ob2" "/O2 /Ob2")
        list(GET _msvc_opt_flags ${RAYTRACER_OPT_LEVEL} _opt_flag)
        set(CMAKE_CXX_FLAGS_RELEASE "${_opt_flag} /DNDEBUG")
    else ()
        set(CMAKE_CXX_FLAGS_RELEASE "-O${RAYTRACER_OPT_LEVEL} -DNDEBUG")
    endif ()
endif ()

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
