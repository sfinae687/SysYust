# InterpreterTest

# 自动测试

add_executable(InterpreterTest InterpreterTest.cpp)
target_link_libraries(InterpreterTest PRIVATE Interpreter)

list(LENGTH sy_source_list test_case_number)
if (${EnableInterpreterTest} AND NOT ${test_case_number} EQUAL 0)
    math(EXPR test_case_number "${test_case_number} - 1")
    foreach (ind RANGE ${test_case_number})
        list(GET sy_source_list ${ind} sy_source_path)
        string(REGEX REPLACE "(.*)\.sy" "\\1" sy_source_stem ${sy_source_path})
        list(GET sy_test_name_list ${ind} sy_test_name)
        set(sy_test_name Interpret_${sy_test_name})
        set(sy_test_input_path ${sy_source_stem}.in)
        set(sy_test_output_path ${sy_source_stem}.out)
        set(sy_test_program_output ${sy_test_name}.out)
        if (NOT EXISTS ${sy_test_input_path} AND NOT EXISTS ${sy_test_output_path})
            message("missing test case for ${sy_source_path}")
            continue()
        else()
            set(rd_input "<${sy_test_input_path}")
            if (NOT EXISTS ${sy_test_input_path})
                unset(rd_input)
            endif ()
            add_test(NAME ${sy_test_name}
                COMMAND bash -c "$<TARGET_FILE:InterpreterTest> ${sy_source_path} ${rd_input} >${sy_test_program_output} &&
                        $<TARGET_FILE:CompareBySymbol> ${sy_test_output_path} ${sy_test_program_output} ||
                        diff ${sy_test_output_path} ${sy_test_program_output}"
            )
            set_tests_properties(${sy_test_name} PROPERTIES
                FAIL_REGULAR_EXPRESSION "FAIL"
            )
        endif ()
    endforeach ()
endif ()