macro(add_etap_test test_name)
    qt_add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE
        etapkitap_core
        Qt6::Test
    )
    target_compile_definitions(${test_name} PRIVATE
        PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

add_etap_test(test_manifest)
add_etap_test(test_security)
add_etap_test(test_annotations)
add_etap_test(test_storage)
add_etap_test(test_activities)
