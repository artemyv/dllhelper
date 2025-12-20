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
target_compile_features(dllhelper PUBLIC cxx_std_20)

target_include_directories(dllhelper 
    PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(dllhelper PRIVATE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
  "$<${msvc_cxx}:-W4>"
)
if(DLLHELPER_USE_GSL)
    include(FetchContent)
    FetchContent_Declare(
        GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG        v4.2.0
        GIT_SHALLOW    ON
    )
    FetchContent_MakeAvailable(GSL)
    target_compile_definitions(dllhelper PUBLIC WITH_GSL)
    target_link_libraries(dllhelper PUBLIC Microsoft.GSL::GSL)
endif()

INSTALL(TARGETS dllhelper 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
