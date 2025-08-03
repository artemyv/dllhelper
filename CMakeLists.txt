project(dllhelper)
cmake_minimum_required(VERSION 3.15)

add_executable(example_in_c
	src/example.c
)

add_executable(example_in_cpp
	src/example.cpp
	include/dllhelper.hpp
)
target_include_directories(example_in_cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(dllhelper INTERFACE)
set_target_properties(dllhelper PROPERTIES PUBLIC_HEADER "include/dllhelper.hpp")
target_include_directories(dllhelper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

INSTALL(TARGETS dllhelper 
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
