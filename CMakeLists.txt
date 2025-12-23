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

# Define install dir variables early for subdirectories
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Detect Windows architecture tag and OS dir for install layout (must be before subdirectories)
set(DLLHELPER_ARCH_TAG "")
set(DLLHELPER_OS_DIR "${CMAKE_SYSTEM_NAME}")
if(WIN32)
    set(DLLHELPER_OS_DIR "win32")
endif()

if(DEFINED CMAKE_GENERATOR_PLATFORM AND CMAKE_GENERATOR_PLATFORM)
    string(TOLOWER "${CMAKE_GENERATOR_PLATFORM}" _dllhelper_genplat)
    if(_dllhelper_genplat STREQUAL "win32")
        set(DLLHELPER_ARCH_TAG "x86")
    elseif(_dllhelper_genplat STREQUAL "x64")
        set(DLLHELPER_ARCH_TAG "x64")
    elseif(_dllhelper_genplat STREQUAL "arm64")
        set(DLLHELPER_ARCH_TAG "arm64")
    else()
        set(DLLHELPER_ARCH_TAG "${CMAKE_GENERATOR_PLATFORM}")
    endif()
else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(DLLHELPER_ARCH_TAG "x86")
    else()
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64|aarch64")
            set(DLLHELPER_ARCH_TAG "arm64")
        else()
            set(DLLHELPER_ARCH_TAG "x64")
        endif()
    endif()
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
  FILE dllhelper-targets-${DLLHELPER_OS_DIR}.${DLLHELPER_ARCH_TAG}.cmake
)
