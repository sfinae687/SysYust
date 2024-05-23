add_executable(BuildSystemTest BuildSystemTest.cpp)
target_link_libraries(BuildSystemTest gtest GTest::gtest_main fmt::fmt argparse)

add_executable(LoggerTest LoggerTest.cpp)
target_link_libraries(LoggerTest gtest GTest::gtest_main utility)

gtest_discover_tests(BuildSystemTest)
gtest_discover_tests(LoggerTest
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
