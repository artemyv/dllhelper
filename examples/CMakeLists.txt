if(WIN32)
    set(ALL_EXAMPLES win)
else()
    set(ALL_EXAMPLES linux)
endif()

message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(vart.dllhelper.examples.${example})
    target_sources(vart.dllhelper.examples.${example} PRIVATE ${example}.cpp)
    target_link_libraries(
        vart.dllhelper.examples.${example}
        PRIVATE vart::dllhelper
    )
endforeach()
