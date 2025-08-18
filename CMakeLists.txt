cmake_minimum_required(VERSION 3.15)

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
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

include(FetchContent)
if(DLLHELPER_USE_GSL)
    FetchContent_Declare(
        GSL
        GIT_REPOSITORY https://github.com/microsoft/GSL.git
        GIT_TAG        v4.2.0 # or a commit hash or branch
    )
    FetchContent_MakeAvailable(GSL)
    add_compile_definitions(WITH_GSL)
endif()

add_subdirectory("src/library")

add_subdirectory("src/samples")

if(DLLHELPER_BUILD_TESTS)
    enable_testing()
    add_subdirectory("test")
endif()

INSTALL(TARGETS dllhelper 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(DLLHELPER_MAIN_PROJECT)
    # Add a custom target for Cppcheck
    find_program(CPPCHECK_EXECUTABLE cppcheck)

    if(CPPCHECK_EXECUTABLE )
        add_custom_target(run_cppcheck
            COMMAND ${CPPCHECK_EXECUTABLE}
            --enable=all
            --inconclusive
            --quiet
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running Cppcheck static analysis..."
            VERBATIM
        )
    else()
        message(WARNING "Cppcheck not found. Please install it to enable static analysis.")
    endif()
endif()
