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
add_executable(tests tests.cpp)

# Link against gtest_main (provides main())
target_link_libraries(tests PRIVATE GTest::gtest_main)

# Enable CTest and test discovery
include(GoogleTest)
gtest_discover_tests(tests)
