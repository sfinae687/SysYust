add_executable(IR_TypeTest TypeTest.cpp)
target_link_libraries(IR_TypeTest IR GTest::gtest_main)
gtest_discover_tests(IR_TypeTest)

add_executable(IR_InstructionTest InstructionTest.cpp)
target_link_libraries(IR_InstructionTest IR GTest::gtest_main)
gtest_discover_tests(IR_InstructionTest)

add_executable(IR_CodeBuildMixinTest CodeBuildMixinTest.cpp)
target_link_libraries(IR_CodeBuildMixinTest IR GTest::gtest_main)
gtest_discover_tests(IR_InstructionTest)

add_executable(IR_DUTEST DU_Test.cpp)
target_link_libraries(IR_DUTEST IR GTest::gtest_main)
gtest_discover_tests(IR_DUTEST)