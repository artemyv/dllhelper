cmake_minimum_required(VERSION 4.2)

project(dllhelper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DLLHELPER_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(DLLHELPER_MAIN_PROJECT ON)
endif()

option(DLLHELPER_BUILD_TESTS   "Build the unit tests" ${DLLHELPER_MAIN_PROJECT})
option(DLLHELPER_BUILD_SAMPLES "Build the examples"   ${DLLHELPER_MAIN_PROJECT})

option(DLLHELPER_USE_GSL       "Use GSL library" ON)

# Ensure compile_commands.json is generated
set(CMAKE_EXPORT_COMPILE_COMMANDS  ${DLLHELPER_MAIN_PROJECT})

include(FetchContent)
if(DLLHELPER_USE_GSL)
    FetchContent_Declare(
        GSL
        GIT_REPOSITORY https://github.com/microsoft/GSL.git
        GIT_TAG        v4.2.0 # or a commit hash or branch
    )
    FetchContent_MakeAvailable(GSL)
endif()

add_subdirectory("src/library")
if(DLLHELPER_BUILD_SAMPLES)
    add_subdirectory("src/samples")
endif()

if(DLLHELPER_BUILD_TESTS)
    add_subdirectory("test")
endif()

INSTALL(TARGETS dllhelper 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

