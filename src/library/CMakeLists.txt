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

target_sources(dllhelper
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
        FILES 
            ${PROJECT_SOURCE_DIR}/include/dllhelper.h
)

target_compile_features(dllhelper PUBLIC cxx_std_20)
set_target_properties(dllhelper PROPERTIES CXX_EXTENSIONS OFF)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(dllhelper PRIVATE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
  "$<${msvc_cxx}:-W4>"
)
if(DLLHELPER_USE_GSL)
    find_package(Microsoft.GSL CONFIG QUIET)
    if(Microsoft.GSL_FOUND)
        # If found as imported, it is safe to expose publicly
        target_link_libraries(dllhelper INTERFACE Microsoft.GSL::GSL)
        target_compile_definitions(dllhelper INTERFACE WITH_GSL)
    endif()
endif()

install(TARGETS dllhelper 
        EXPORT  dllhelper-targets
        FILE_SET HEADERS
)
