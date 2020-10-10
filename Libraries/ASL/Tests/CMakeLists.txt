file(GLOB ASL_TEST_SOURCES "*.cpp")

foreach(source ${ASL_TEST_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})    
    add_test(
        NAME ${name}
        COMMAND ${name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    set_tests_properties(
        ${name}
        PROPERTIES
            FAIL_REGULAR_EXPRESSION
            "FAIL"
    )
endforeach()