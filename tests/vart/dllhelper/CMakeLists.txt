#Download and make GTest available
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # release-1.17.0
    FIND_PACKAGE_ARGS NAMES GTest
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Add a new UT project using GTest
add_executable(dllhelper_ut
    dllhelper.ut.cpp
)

# Link GTest and the necessary libraries
target_link_libraries(dllhelper_ut
    PRIVATE
        vart::dllhelper
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dllhelper_ut)

add_library(mock_dll SHARED)

target_sources(mock_dll
    PRIVATE
        dll_helper_mock.cpp
)

# Ensure the mock DLL is placed next to the test executable
add_dependencies(dllhelper_ut mock_dll)

# Provide the full path to the built mock_dll as a string literal
target_compile_definitions(dllhelper_ut PRIVATE
    MOCK_DLL_PATH="$<TARGET_FILE:mock_dll>"
)
