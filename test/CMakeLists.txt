function(add_unit_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE unity)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_unit_test(test_range test_range.c)
