function(AddGTestFromFile testName)
    add_executable(${testName} ${testName}.cpp)
    target_link_libraries(${testName} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${testName})
endfunction()

add_executable(BuildSystemTest BuildSystemTest.cpp)
target_link_libraries(BuildSystemTest gtest GTest::gtest_main fmt::fmt argparse)

add_executable(LoggerTest LoggerTest.cpp)
target_link_libraries(LoggerTest gtest GTest::gtest_main utility)

add_executable(TypeMatchTest TypeMatchTest.cpp)
target_link_libraries(TypeMatchTest GTest::gtest_main AST)

add_executable(TypeCreateTest TypeCreateTest.cpp)
target_link_libraries(TypeCreateTest GTest::gtest_main AST)

add_executable(NodeTest NodeTest.cpp)
target_link_libraries(NodeTest GTest::gtest_main AST)

AddGTestFromFile(TypeToStringTest)
target_link_libraries(TypeToStringTest PUBLIC AST)

gtest_discover_tests(BuildSystemTest)
gtest_discover_tests(LoggerTest)
gtest_discover_tests(TypeMatchTest)
gtest_discover_tests(TypeCreateTest)
gtest_discover_tests(NodeTest)