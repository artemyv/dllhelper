

if(WIN32)
    add_executable(example_in_c
        "example_win.c"
    )

    add_executable(example_in_cpp
        "example_win.cpp"
    )
else()
    add_executable(example_in_cpp
        "example_linux.cpp"
    )
endif()
target_sources(example_in_cpp PRIVATE
    FILE_SET HEADERS
    BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
)
# Link example_in_cpp with dllhelper static library
target_link_libraries(example_in_cpp PRIVATE vart::dllhelper)

if(_dllhelper_code_coverage )
    target_compile_options(example_in_cpp PRIVATE --coverage -O0)
    target_link_options(example_in_cpp    PRIVATE --coverage)
endif()