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
    dll_helper_mock.cpp
)

# Link GTest and the necessary libraries
target_link_libraries(dllhelper_ut
    PRIVATE
        dllhelper
        GTest::gtest_main
)
if(_dllhelper_code_coverage )
    target_compile_options(dllhelper_ut PRIVATE --coverage -O0)
    target_link_options(dllhelper_ut    PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(dllhelper_ut)
