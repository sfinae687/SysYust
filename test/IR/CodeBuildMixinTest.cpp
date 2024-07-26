//
// Created by LL06p on 24-7-26.
//
#include <gtest/gtest.h>

#include "utility/StreamLogger.h"
#include "IR/CodeUtil.h"

using namespace SysYust::IR;


class TestCodeBuilder : public ::testing::Test, protected CodeBuildMixin {
public:
    TestCodeBuilder() {
        setup_code();
        SysYust::Logger::setLogger(new SysYust::StreamLogger(std::clog));
    }
    ~TestCodeBuilder() override {
        delete getCode();
        SysYust::Logger::setLogger(nullptr);
    }
};

TEST_F(TestCodeBuilder, ProcedureTest) {
    EXPECT_FALSE(current_procedure());
    {
        entry_function({"fac"}, {Type::get(SysYust::IR::Type::i)});
        EXPECT_TRUE(current_procedure());
        EXPECT_EQ(current_procedure()->name().full(), "fac");
        EXPECT_EQ(current_procedure()->type(), Type::get(SysYust::IR::Type::i));
        exit_function();
        EXPECT_FALSE(current_procedure());
    }
    {
        function_guard main_func(this, {"main"}, {Type::get(SysYust::IR::Type::none)});
        EXPECT_TRUE(current_procedure());
    }
    EXPECT_FALSE(current_procedure());
}

TEST_F(TestCodeBuilder, BasicBlockTest) {
    function_guard main_func(this, {"main"}, {Type::get(SysYust::IR::Type::none)});
    ASSERT_TRUE(current_procedure());

    EXPECT_FALSE(current_block());
    entry_block();
    EXPECT_TRUE(current_block());

    EXPECT_EQ(current_procedure()->getGraph().head(), current_block());

    auto headBlock = current_block();

    push_block();
    entry_block();
    EXPECT_NE(headBlock, current_block());
    pop_block();
    EXPECT_EQ(headBlock, current_block());

    save_and_next();
    auto nextBlock = current_block();
    EXPECT_NE(headBlock, nextBlock);
    pop_block();
    EXPECT_EQ(headBlock, current_block());
    entry_next();
    EXPECT_EQ(current_block(), nextBlock);
    EXPECT_EQ(current_block()->prevBlocks().front(), headBlock);
    entry_block(headBlock);
    EXPECT_EQ(headBlock, current_block());

    save_and_else();
    auto elseBlock = current_block();
    EXPECT_NE(headBlock, elseBlock);
    pop_block();
    EXPECT_EQ(headBlock, current_block());
    entry_else();
    EXPECT_EQ(current_block(), elseBlock);
    entry_block(headBlock);
    EXPECT_EQ(headBlock, current_block());

}

TEST_F(TestCodeBuilder, InstTest) {
    function_guard main_func(this, {"main"}, {Type::get(SysYust::IR::Type::i)});
    entry_block();

    auto mem = auto_inst<alc>(Type::get(SysYust::IR::Type::i));
    EXPECT_EQ(mem->type->id(), Type::i);
    EXPECT_EQ(mem.inst.cateId, instruct_cate::alloc);
    EXPECT_EQ(mem->assigned.type, Type::get(SysYust::IR::Type::ptr, Type::get(SysYust::IR::Type::i)));

    mem.push();
    EXPECT_NE(current_block()->begin(), current_block()->end());

    auto rt = auto_inst<call>(func_symbol{"main"}, arg_list{});
    EXPECT_EQ(rt->assigned.type, Type::get(SysYust::IR::Type::i));

}