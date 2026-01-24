# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.28...4.2)

project(
    vart.dllhelper
    DESCRIPTION "How to GetProcAddress like a boss"
    LANGUAGES CXX
    VERSION 3.0.1
)

# [CMAKE.SKIP_TESTS]
option(
    VART_DLLHELPER_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# [CMAKE.SKIP_EXAMPLES]
option(
    VART_DLLHELPER_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

option(VART_DLLHELPER_USE_GSL "Use GSL library" ON)
# Default bad-alloc test toggle depends on sanitizer usage
set(_dllhelper_badalloc_default ON)

# Respect external build system sanitizer knobs
if(
    DEFINED BEMAN_BUILDSYS_SANITIZER
    AND BEMAN_BUILDSYS_SANITIZER STREQUAL "MaxSan"
)
    set(_dllhelper_badalloc_default OFF)
endif()
if(
    DEFINED BEMAN_BUILDSYS_SANITIZER
    AND BEMAN_BUILDSYS_SANITIZER STREQUAL "TsSan"
)
    set(_dllhelper_badalloc_default OFF)
endif()
# Project-specific sanitizer selection
if(
    DEFINED DLLHELPER_BUILDSYS_SANITIZER
    AND DLLHELPER_BUILDSYS_SANITIZER STREQUAL "MaxSan"
)
    set(_dllhelper_badalloc_default OFF)
endif()
if(
    DEFINED DLLHELPER_BUILDSYS_SANITIZER
    AND DLLHELPER_BUILDSYS_SANITIZER STREQUAL "TsSan"
)
    set(_dllhelper_badalloc_default OFF)
endif()

option(
    VART_DLLHELPER_TEST_BADALLOC
    "Build bad-alloc test target that overrides global new/delete. Default depends on sanitizer flags."
    ${_dllhelper_badalloc_default}
)

add_subdirectory("src/vart/dllhelper")
add_subdirectory("include/vart/dllhelper")

if(VART_DLLHELPER_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

if(VART_DLLHELPER_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests/vart/dllhelper")
endif()

# for find of vart-install-library
include(infra/cmake/vart-install-library-config.cmake)

vart_install_library(vart.dllhelper)
