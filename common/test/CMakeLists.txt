file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(raytracer-common-test ${SOURCES})
add_executable(raytracer::common::test ALIAS raytracer-common-test)

target_link_libraries(raytracer-common-test
        PRIVATE
        raytracer::common
        GTest::gtest_main
)

gtest_discover_tests(raytracer-common-test)
