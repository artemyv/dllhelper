cmake_minimum_required(VERSION 4.2)

project(
	vart.dllhelper
        DESCRIPTION "How to GetProcAddress like a boss"
        LANGUAGES CXX C
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
option(INSTALL_GTEST "Enable installation of googletest." OFF)



add_subdirectory("src/vart/dllhelper")

if(VART_DLLHELPER_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

if(VART_DLLHELPER_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests/vart/dllhelper")
endif()

# todo - use same approach as beman_install_library(beman.exemplar)
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
  EXPORT vart_dllhelper-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dllhelper
  NAMESPACE vart::dllhelper::
  FILE vart_dllhelper-targets-${DLLHELPER_OS_DIR}.${DLLHELPER_ARCH_TAG}.cmake
)
