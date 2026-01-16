cmake_minimum_required(VERSION 3.28...4.2)

project(
	vart.dllhelper
        DESCRIPTION "How to GetProcAddress like a boss"
        LANGUAGES CXX
	VERSION 3.0.0
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

option(DLLHELPER_USE_GSL       "Use GSL library"      ON)

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