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
foreach(sy_source ${sy_source_list})
    file(RELATIVE_PATH sy_path ${sy_source_root} ${sy_source})
    string(REPLACE "/" "_" sy_name ${sy_path})
    string(REGEX REPLACE "(.*)\.sy" "\\1" sy_name ${sy_name})
#    message("Find test case ${sy_name}")
    list(APPEND sy_test_name_list ${sy_name})
endforeach ()

# 语法树构建测试
add_subdirectory(AST-BuilderTest)
add_subdirectory(InterpreterTest)