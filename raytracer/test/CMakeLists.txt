file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(raytracer-raytracer-test ${SOURCES})
add_executable(raytracer::raytracer::test ALIAS raytracer-raytracer-test)

target_link_libraries(raytracer-raytracer-test
        PRIVATE
        raytracer::raytracer
        GTest::gtest_main
)

gtest_discover_tests(raytracer-raytracer-test)
