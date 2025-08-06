cmake_minimum_required(VERSION 3.15)

project(dllhelper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

set(LIB_SOURCES)
if(WIN32)
    list(APPEND LIB_SOURCES src/dll_helper_win.cpp)
elseif(UNIX AND NOT APPLE)
    list(APPEND LIB_SOURCES src/dll_helper_linux.cpp)
endif()

# Create static library instead of interface library
add_library(dllhelper STATIC
    ${LIB_SOURCES}
)
target_include_directories(dllhelper 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(dllhelper PROPERTIES 
    PUBLIC_HEADER "include/dllhelper.hpp"
)

# Only build examples if this is the top-level project
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    if(WIN32)
        add_executable(example_in_c
            "src/example_win.c"
         )

        add_executable(example_in_cpp
            "src/example_win.cpp"
            include/dllhelper.hpp)
    
        target_compile_options(example_in_cpp PRIVATE /W4)
    else()
        add_executable(example_in_cpp
            "src/example_linux.cpp"
            include/dllhelper.hpp)
        target_compile_options(example_in_cpp PRIVATE -Wall)
    endif()
    target_include_directories(example_in_cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    # Link example_in_cpp with dllhelper static library
    target_link_libraries(example_in_cpp PRIVATE dllhelper)
    enable_testing()

    # Add a new UT project using GTest
    add_executable(dllhelper_ut
        test/dllhelper.ut.cpp
        test/dll_helper_mock.cpp
    )

    # Link GTest and the necessary libraries
    target_link_libraries(dllhelper_ut
        PRIVATE
            gtest
            gtest_main
    )
    target_include_directories(dllhelper_ut 
        PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # Add GTest discovery for CTest
    include(GoogleTest)
    gtest_discover_tests(dllhelper_ut)

    add_custom_target(run_tests ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS dllhelper_ut
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
endif()


INSTALL(TARGETS dllhelper 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
