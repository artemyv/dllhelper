project(dllhelper)
cmake_minimum_required(VERSION 3.5)

add_executable(example_in_c
	example.c
)

add_executable(example_in_cpp
	example.cpp
	dllhelper.hpp
)