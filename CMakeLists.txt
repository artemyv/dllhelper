cmake_minimum_required(VERSION 4.2)

project(dllhelper VERSION 3.0.0
                  DESCRIPTION "How to GetProcAddress like a boss"
                  LANGUAGES CXX C)

set(DLLHELPER_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(DLLHELPER_MAIN_PROJECT ON)
endif()

# Default DLLHELPER_BUILD_TESTS:
# - If BUILD_TESTING is undefined: use DLLHELPER_MAIN_PROJECT
# - If BUILD_TESTING is defined: use its value
set(_dllhelper_default_tests ${DLLHELPER_MAIN_PROJECT})
if(DEFINED BUILD_TESTING)
    if(BUILD_TESTING)
        set(_dllhelper_default_tests ON)
    else()
        set(_dllhelper_default_tests OFF)
    endif()
endif()
option(DLLHELPER_BUILD_TESTS "Build the unit tests"   ${_dllhelper_default_tests})

option(DLLHELPER_BUILD_SAMPLES "Build the examples"   ${DLLHELPER_MAIN_PROJECT})
option(DLLHELPER_USE_GSL       "Use GSL library"      ON)
option(INSTALL_GTEST "Enable installation of googletest." OFF)

# Ensure compile_commands.json is generated
set(CMAKE_EXPORT_COMPILE_COMMANDS  ${DLLHELPER_MAIN_PROJECT})

# Coverage option
set(_dllhelper_code_coverage OFF)
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    set(_dllhelper_code_coverage ON)
endif()

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

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dllhelper-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/dllhelper-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dllhelper
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/dllhelper-version.cmake"
  COMPATIBILITY ExactVersion
)

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/dllhelper-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/dllhelper-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dllhelper
)

install(
  EXPORT dllhelper-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dllhelper
  NAMESPACE dllhelper::
  FILE dllhelper-targets.cmake
)

