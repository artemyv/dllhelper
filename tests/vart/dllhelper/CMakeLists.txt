find_package(GTest REQUIRED)

# Add a new UT project using GTest
add_executable(vart.dllhelper.tests.dllhelper)

target_sources(vart.dllhelper.tests.dllhelper PRIVATE dllhelper.test.cpp)

# Link GTest and the necessary libraries
target_link_libraries(vart.dllhelper.tests.dllhelper
    PRIVATE
        vart::dllhelper
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(vart.dllhelper.tests.dllhelper)

add_library(vart.dllhelper.tests.mock_dll SHARED)

target_sources(vart.dllhelper.tests.mock_dll
    PRIVATE
        dllhelper.mock.cpp
)

# Ensure the mock DLL is placed next to the test executable
add_dependencies(vart.dllhelper.tests.dllhelper vart.dllhelper.tests.mock_dll)

# Provide the full path to the built mock_dll as a string literal
target_compile_definitions(vart.dllhelper.tests.dllhelper PRIVATE
    MOCK_DLL_PATH="$<TARGET_FILE:vart.dllhelper.tests.mock_dll>"
)
