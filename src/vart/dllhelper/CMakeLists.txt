set(LIB_SOURCES)
if(WIN32)
    list(APPEND LIB_SOURCES dll_helper_win.cpp)
elseif(UNIX AND NOT APPLE)
    list(APPEND LIB_SOURCES dll_helper_linux.cpp)
endif()

# Create static library instead of interface library
add_library(vart.dllhelper) 
add_library(vart::dllhelper ALIAS vart.dllhelper)

target_sources(vart.dllhelper
    PRIVATE
        ${LIB_SOURCES}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(vart.dllhelper PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)
   
if(DLLHELPER_USE_GSL)
    find_package(Microsoft.GSL CONFIG QUIET)
    if(Microsoft.GSL_FOUND)
        # If found as imported, it is safe to expose publicly
        target_link_libraries(vart.dllhelper INTERFACE Microsoft.GSL::GSL)
        target_compile_definitions(vart.dllhelper INTERFACE WITH_GSL)
    endif()
endif()

install(TARGETS vart.dllhelper
    EXPORT vart_dllhelper-targets
    FILE_SET HEADERS)
