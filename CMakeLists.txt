cmake_minimum_required(VERSION 3.15)

project(dllhelper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
endif()

INSTALL(TARGETS dllhelper 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
