project(dllhelper)
cmake_minimum_required(VERSION 3.15)

add_executable(example_in_c
	example.c
)

add_executable(example_in_cpp
	example.cpp
	dllhelper.hpp
)

add_library(dllhelper INTERFACE)
set_target_properties(dllhelper PROPERTIES PUBLIC_HEADER "dllhelper.hpp")

INSTALL(TARGETS dllhelper 
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dllhelper
)
