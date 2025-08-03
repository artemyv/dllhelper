cmake_minimum_required(VERSION 3.15)

project(dllhelper)

# Only build examples if this is the top-level project
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_executable(example_in_c
        src/example.c
    )

    add_executable(example_in_cpp
        src/example.cpp
        include/dllhelper.hpp
    )
    target_include_directories(example_in_cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(is_gnu_like "$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>")
    set(is_msvc "$<CXX_COMPILER_ID:MSVC>")
    target_compile_options(example_in_cpp PRIVATE
         "$<${is_gnu_like}:-Wall>"
         "$<${is_msvc}:/W4>"
        )
endif()

add_library(dllhelper INTERFACE)
set_target_properties(dllhelper PROPERTIES PUBLIC_HEADER "include/dllhelper.hpp")
target_include_directories(dllhelper SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

INSTALL(TARGETS dllhelper 
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
