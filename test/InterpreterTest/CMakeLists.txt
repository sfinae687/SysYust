# InterpreterTest

# 自动测试

add_executable(InterpreterTest InterpreterTest.cpp)
target_link_libraries(InterpreterTest PRIVATE Interpreter)

list(LENGTH sy_source_list test_case_number)
if (${EnableInterpreterTest} AND NOT ${test_case_number} EQUAL 0)
    math(EXPR test_case_number "${test_case_number} - 1")
    foreach (ind RANGE ${test_case_number})
        list(GET sy_source_list ${ind} sy_source)
        list(GET sy_test_name_list ${ind} sy_name)
        set(sy_name Interpreter_${sy_name})
        list(GET sy_test_output_list ${ind} sy_output)
        list(GET sy_test_input_list ${ind} sy_input)
        list(GET sy_test_ret_list ${ind} sy_ret)
        list(GET sy_test_exe_list ${ind} sy_executable)
        string(REGEX REPLACE "(.*)\.sy" "\\1" sy_source_stem ${sy_source})
        set(sy_program_output ${sy_name}.out)
        set(sy_program_ret ${sy_name}.ret)
        if (NOT EXISTS ${sy_input} AND NOT EXISTS ${sy_output})
            message("missing test case for ${sy_source}")
            continue()
        else()
            set(rd_input "<${sy_input}")
            if (NOT EXISTS ${sy_input})
                unset(rd_input)
            endif ()
            add_test(NAME ${sy_name}
                COMMAND bash -c "
                    cp ${sy_source} ${CMAKE_CURRENT_BINARY_DIR}/${sy_name}.cpp
                    ${CMAKE_CXX_COMPILER} -std=c++17 -include ${sy_header} -o ${sy_executable} ${CMAKE_CURRENT_BINARY_DIR}/${sy_name}.cpp ${sy_lib}
                    ${sy_executable} ${rd_input} >${sy_output}
                    echo $? >${sy_ret}
                    $<TARGET_FILE:InterpreterTest> ${sy_source} ${rd_input} >${sy_program_output}
                    echo $? >${sy_program_ret} && {
                        $<TARGET_FILE:CompareBySymbol> ${sy_output} ${sy_program_output}
                        if [  $? -ne 0 ];
                        then
                            echo OUTPUT DIFFERENT
                            diff ${sy_output} ${sy_program_output}
                            exit 1
                        fi
                    } && {
                        $<TARGET_FILE:CompareBySymbol> ${sy_ret} ${sy_program_ret}
                        if [  $? -ne 0 ];
                        then
                            echo OUTPUT DIFFERENT
                            diff ${sy_ret} ${sy_program_ret}
                            exit 1
                        fi
                    }
                    exit 0
                    "
            )
            set_tests_properties(${sy_name} PROPERTIES
                FAIL_REGULAR_EXPRESSION "FAIL"
            )
        endif ()
    endforeach ()
endif ()