cmake_minimum_required(VERSION 4.2)

project(dllhelper VERSION 3.0.0
                  DESCRIPTION "How to GetProcAddress like a boss"
                  LANGUAGES CXX C)

set(DLLHELPER_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(DLLHELPER_MAIN_PROJECT ON)
endif()

option(DLLHELPER_BUILD_TESTS   "Build the unit tests" ${DLLHELPER_MAIN_PROJECT})
option(DLLHELPER_BUILD_SAMPLES "Build the examples"   ${DLLHELPER_MAIN_PROJECT})
option(DLLHELPER_USE_GSL       "Use GSL library"      ON)

# Ensure compile_commands.json is generated
set(CMAKE_EXPORT_COMPILE_COMMANDS  ${DLLHELPER_MAIN_PROJECT})

add_subdirectory("src/library")
if(DLLHELPER_BUILD_SAMPLES)
    add_subdirectory("src/samples")
endif()

if(DLLHELPER_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory("test")

    # Run tests automatically after build
    add_custom_target(run_dllhelper_tests ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMENT "Running unit tests for dllhelper library..."
    )
    add_dependencies(run_dllhelper_tests dllhelper_ut)
endif()

INSTALL(TARGETS dllhelper 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

