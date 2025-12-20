#Download and make GTest available
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Add a new UT project using GTest
add_executable(dllhelper_ut
    dllhelper.ut.cpp
    dll_helper_mock.cpp
)

# Link GTest and the necessary libraries
target_link_libraries(dllhelper_ut
    PRIVATE
        dllhelper
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dllhelper_ut)

