# This toolchain file is not meant to be used directly,
# but to be invoked by CMake preset and GitHub CI.
#
# This toolchain file configures for apple clang family of compiler.
# Note this is different from LLVM toolchain.
#
# DLLHELPER_BUILDSYS_SANITIZER:
# This optional CMake parameter is not meant for public use and is subject to
# change.
# Possible values:
# - MaxSan: configures clang and clang++ to use all available non-conflicting
#           sanitizers. Note that apple clang does not support leak sanitizer.
# - TSan:   configures clang and clang++ to enable the use of thread sanitizer.

include_guard(GLOBAL)

# Find compilers
find_program(CMAKE_C_COMPILER clang)
find_program(CMAKE_CXX_COMPILER clang++)
find_program(CMAKE_OBJC_COMPILER clang)
if(NOT CMAKE_C_COMPILER OR NOT CMAKE_CXX_COMPILER OR NOT CMAKE_OBJC_COMPILER)
    message(FATAL_ERROR "Clang compilers not found. Ensure clang is installed and in PATH.")
endif()

if(DLLHELPER_BUILDSYS_SANITIZER STREQUAL "MaxSan")
    set(SANITIZER_FLAGS
        "-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined"
    )
elseif(DLLHELPER_BUILDSYS_SANITIZER STREQUAL "TSan")
    set(SANITIZER_FLAGS "-fsanitize=thread")
endif()

set(CMAKE_C_FLAGS_DEBUG_INIT "${SANITIZER_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${SANITIZER_FLAGS}")

set(RELEASE_FLAGS "-O3 ${SANITIZER_FLAGS}")

set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "${RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "${RELEASE_FLAGS}")

set(CMAKE_C_FLAGS_RELEASE_INIT "${RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${RELEASE_FLAGS}")

# Add this dir to the module path so that `find_package(beman-install-library)` works
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}")
