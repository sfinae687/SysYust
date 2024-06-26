/// @file 创建用于测试构建系统是否正常工作的TestSuit的文件

#include <gtest/gtest.h>
#include <fmt/core.h>
#include <argparse/argparse.hpp>

TEST(BuildSystemTest, GTestTest) {
    ASSERT_TRUE(true) << "Google Test has been installed correctly";
}

TEST(BuildSystemTest, fmtTest) {
    ASSERT_EQ("FormatInt:0", fmt::format("FormatInt:{}", 0));
}

TEST(BuildSystemTest, argparseTest) {
    argparse::ArgumentParser program("ParserTest");
    program.add_argument("aInt").scan<'i',int>();
    program.parse_args({"./main", "0"});
    ASSERT_EQ(program.get<int>("aInt"), 0);
}