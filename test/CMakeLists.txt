add_executable(BuildSystemTest BuildSystemTest.cpp)
target_link_libraries(BuildSystemTest gtest GTest::gtest_main fmt::fmt argparse)

add_executable(LoggerTest LoggerTest.cpp)
target_link_libraries(LoggerTest gtest GTest::gtest_main utility)

add_executable(TypeMatchTest TypeMatchTest.cpp)
target_link_libraries(TypeMatchTest GTest::gtest_main AST)

gtest_discover_tests(BuildSystemTest)
gtest_discover_tests(LoggerTest)
gtest_discover_tests(TypeMatchTest)