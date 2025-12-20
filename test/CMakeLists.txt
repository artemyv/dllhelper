#Download and make GTest available
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add a new UT project using GTest
add_executable(dllhelper_ut
    dllhelper.ut.cpp
    dll_helper_mock.cpp
)

# Link GTest and the necessary libraries
target_link_libraries(dllhelper_ut
    PRIVATE
        dllhelper
        gtest
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(dllhelper_ut)

