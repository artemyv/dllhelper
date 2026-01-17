# SPDX-License-Identifier: MIT

add_library(vart.dllhelper)
add_library(vart::dllhelper ALIAS vart.dllhelper)

set_target_properties(vart.dllhelper PROPERTIES LINKER_LANGUAGE CXX)

if(WIN32)
    target_sources(vart.dllhelper PRIVATE dll_helper_win.cpp)
else()
    target_sources(vart.dllhelper PRIVATE dll_helper_linux.cpp)
endif()

target_sources(
    vart.dllhelper
    PUBLIC FILE_SET HEADERS BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(vart.dllhelper PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

if(DLLHELPER_USE_GSL)
    find_package(Microsoft.GSL CONFIG QUIET)
    if(Microsoft.GSL_FOUND)
        # Link GSL only for the build tree to avoid exporting
        # a non-imported target from FetchContent in install(EXPORT).
        if(TARGET Microsoft.GSL::GSL)
            target_link_libraries(
                vart.dllhelper
                PUBLIC $<BUILD_INTERFACE:Microsoft.GSL::GSL>
            )
        elseif(TARGET GSL)
            target_link_libraries(vart.dllhelper PUBLIC $<BUILD_INTERFACE:GSL>)
        endif()
        target_compile_definitions(vart.dllhelper PUBLIC WITH_GSL)
    endif()
endif()

#install(TARGETS vart.dllhelper EXPORT vart_dllhelper-targets FILE_SET HEADERS)
