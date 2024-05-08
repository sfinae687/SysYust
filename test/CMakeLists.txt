add_executable(BuildSystemTest BuildSystemTest.cpp)
target_link_libraries(BuildSystemTest  GTest::gtest_main fmt::fmt argparse)
gtest_discover_tests(BuildSystemTest)