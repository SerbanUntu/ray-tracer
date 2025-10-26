cmake_minimum_required(VERSION 3.20)

include(FetchContent)

# Declare googletest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

# For Windows: prevent overriding parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Download and add googletest
FetchContent_MakeAvailable(googletest)

# Define test executable
add_executable(tests "util.cpp" "json.cpp")

# Link against gtest_main (provides main())
target_link_libraries(tests PRIVATE render_lib)
target_link_libraries(tests PRIVATE GTest::gtest_main)
target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json)

# Enable CTest and test discovery
include(GoogleTest)
gtest_discover_tests(tests)

set_property(TARGET tests PROPERTY CXX_STANDARD 23)
