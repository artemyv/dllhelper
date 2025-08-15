
# Only build examples if this is the top-level project
if(DLLHELPER_BUILD_TESTS)
    if(WIN32)
        add_executable(example_in_c
            "example_win.c"
        )

        add_executable(example_in_cpp
            "example_win.cpp"
        )
    
        target_compile_options(example_in_cpp PRIVATE /W4)
    else()
        add_executable(example_in_cpp
            "example_linux.cpp"
        )
        target_compile_options(example_in_cpp PRIVATE -Wall)
    endif()
    target_include_directories(example_in_cpp PRIVATE ${PROJECT_SOURCE_DIR}/include)
    # Link example_in_cpp with dllhelper static library
    target_link_libraries(example_in_cpp PRIVATE dllhelper)
endif()

