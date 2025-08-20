set(LIB_SOURCES)
if(WIN32)
    list(APPEND LIB_SOURCES dll_helper_win.cpp)
elseif(UNIX AND NOT APPLE)
    list(APPEND LIB_SOURCES dll_helper_linux.cpp)
endif()

# Create static library instead of interface library
add_library(dllhelper STATIC
    ${LIB_SOURCES}
)
target_include_directories(dllhelper 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${gsl_SOURCE_DIR}/include
)
set_target_properties(dllhelper PROPERTIES 
    PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/include/dllhelper.hpp"
)
if(DLLHELPER_USE_GSL)
    target_compile_definitions(dllhelper PUBLIC WITH_GSL)
endif()

INSTALL(TARGETS dllhelper 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
