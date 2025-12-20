option(DLLHELPER_BUILD_TESTS   "Build the unit tests" ${DLLHELPER_MAIN_PROJECT})

if(DLLHELPER_BUILD_TESTS)
    include(FetchContent)

    # Download and make GTest available
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
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
            gtest
            gtest_main
    )
    target_include_directories(dllhelper_ut 
        PRIVATE 
            ${PROJECT_SOURCE_DIR}/include
            ${gsl_SOURCE_DIR}/include
    )

    include(GoogleTest)
    gtest_discover_tests(dllhelper_ut)

    add_custom_target(run_tests ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --verbose
        DEPENDS dllhelper_ut
    )
endif()
