function(AddGTestFromFile testName)
    add_executable(${testName} ${testName}.cpp)
    target_link_libraries(${testName} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${testName})
endfunction()

add_executable(BuildSystemTest BuildSystemTest.cpp)
target_link_libraries(BuildSystemTest gtest GTest::gtest_main fmt::fmt argparse)
gtest_discover_tests(BuildSystemTest)

add_executable(LoggerTest LoggerTest.cpp)
target_link_libraries(LoggerTest gtest GTest::gtest_main utility)
gtest_discover_tests(LoggerTest)

add_executable(TypeMatchTest TypeMatchTest.cpp)
target_link_libraries(TypeMatchTest GTest::gtest_main AST)
gtest_discover_tests(TypeMatchTest)

add_executable(TypeCreateTest TypeCreateTest.cpp)
target_link_libraries(TypeCreateTest GTest::gtest_main AST)
gtest_discover_tests(TypeCreateTest)

add_executable(NodeTest NodeTest.cpp)
target_link_libraries(NodeTest GTest::gtest_main AST)
gtest_discover_tests(NodeTest)

AddGTestFromFile(TypeToStringTest)
target_link_libraries(TypeToStringTest PUBLIC AST)

AddGTestFromFile(ArrayIndexTest)
target_link_libraries(ArrayIndexTest PUBLIC AST)

# 处理 sy 程序测试样例路径
set(sy_test_name_list)
set(sy_test_input_list)
set(sy_test_output_list)
set(sy_test_ret_list)
set(sy_test_exe_list)
foreach(sy_source ${sy_source_list})
    # 根据测试样例，通过 C 编译器重新生成 测试输出
    string(REGEX REPLACE "(.*)\.sy" "\\1" sy_path_stem ${sy_source})
    set(sy_input ${sy_path_stem}.in)
    list(APPEND sy_test_input_list ${sy_input})
    if (EXISTS ${sy_input})
        set(sy_rd_input <${sy_input})
    else ()
        unset(sy_rd_input)
    endif ()
    set(sy_output ${sy_path_stem}.out)
    list(APPEND sy_test_output_list ${sy_output})
    set(sy_ret ${sy_path_stem}.ret)
    list(APPEND sy_test_ret_list ${sy_ret})
    set(sy_executable ${sy_path_stem}.syr)
    list(APPEND sy_test_exe_list ${sy_executable})

    # 生成测试文件的名称
    file(RELATIVE_PATH sy_name ${sy_source_root} ${sy_source})
    string(REPLACE "/" "_" sy_name ${sy_name})
    string(REGEX REPLACE "(.*)\.sy" "\\1" sy_name ${sy_name})
#    message("Find test case ${sy_name}")
    list(APPEND sy_test_name_list ${sy_name})
endforeach ()

# 语法树构建测试
add_subdirectory(AST-BuilderTest)
add_subdirectory(InterpreterTest)
add_subdirectory(IR)
add_subdirectory(IREmitterTest)
add_subdirectory(DomTreeTest)
