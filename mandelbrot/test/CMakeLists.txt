file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(raytracer-mandelbrot-test ${SOURCES})
add_executable(raytracer::mandelbrot::test ALIAS raytracer-mandelbrot-test)

target_link_libraries(raytracer-mandelbrot-test
        PRIVATE
        raytracer::mandelbrot
        GTest::gtest_main
)

gtest_discover_tests(raytracer-mandelbrot-test)
