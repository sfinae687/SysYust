//
// Created by LL06p on 24-8-2.
//

#include <gtest/gtest.h>

#include "IR/CodeUtil.h"
#include "utility/StreamLogger.h"

using namespace SysYust::IR;

class TestCodeBuilder : public ::testing::Test, protected CodeBuildMixin {
public:
    TestCodeBuilder() {
        setup_code();
        SysYust::Logger::setLogger(new SysYust::StreamLogger(std::cout));
    }

    void SetUp() override {

    }

    ~TestCodeBuilder() override {
        delete getCode();
        SysYust::Logger::setLogger(nullptr);
    }
};

TEST_F(TestCodeBuilder, DefineTrack) {
    function_guard mainGuard(this, {"main"}, {Type::get(SysYust::IR::Type::i)});
    entry_block();
    auto block1 = current_block();

    // 指令的符号定义追踪

    auto alc1 = auto_inst<alc>(Type::get(SysYust::IR::Type::i));
    auto alcName = alc1->assigned;
    alc1.push();

    auto opr1 = auto_inst<ld>(alcName);
    auto opr1Name = opr1->assigned;
    opr1.push();

    auto add1 = auto_inst<add>(opr1Name, opr1Name);
    auto add1Name = add1->assigned;
    add1.push();

    // 定义查找
    auto alcDefine = current_du().local_defined(alcName);
    ASSERT_TRUE(alcDefine);
    EXPECT_EQ(alcDefine->defined.var(), alcName);
    EXPECT_EQ(alcDefine->define_type, SymbolDefineType::block);
    EXPECT_EQ(alcDefine->inst_context.index(), 1);
    auto alcDUInst = std::get<DUInst*>(alcDefine->inst_context);
    EXPECT_EQ(alcDUInst->block, current_block());
    EXPECT_EQ(alcDUInst->inst, alc1.pushed_inst_handler());
    EXPECT_EQ(alcDUInst->defineInfo, alcDefine);

    // 立即数定义查找

    auto imDefine = current_du().defined_entry(im_symbol{64});
    EXPECT_EQ(imDefine.define_type, SymbolDefineType::builtin);
    EXPECT_EQ(imDefine.defined.im(), im_symbol{64});

    auto add2 = auto_inst<add>(add1Name, im_symbol{32});
    auto add2Name = add2->assigned;
    add2.push();
    auto add2du = current_du().local_defined(add2Name);
    EXPECT_EQ(std::get<DUInst*>(add2du->inst_context), &current_du().instDUInfo(add2.pushed_inst_handler()));
    auto define32ByArg = add2du->dependence()[1]->defined;
    auto define32BySeek = current_du().local_defined(im_symbol{32});
    EXPECT_EQ(define32ByArg, define32BySeek);

    // 块参数定义查找

    var_symbol bArg = nxt_sym();
    add_block_param(bArg);
    auto bArgDE = current_du().local_defined(bArg);
    EXPECT_TRUE(bArgDE);
    EXPECT_EQ(bArgDE->define_type, SymbolDefineType::block_param);
    EXPECT_EQ(bArgDE->defined.var(), bArg);

    // 函数参数定义查找

    var_symbol fArg = nxt_sym();
    procedure()->add_param(fArg);
    auto fArgDE = current_du().local_defined(fArg);
    EXPECT_TRUE(fArgDE);
    EXPECT_EQ(fArgDE->define_type, SymbolDefineType::func_param);
    EXPECT_EQ(fArgDE->defined.var(), fArg);

}

TEST_F(TestCodeBuilder, UsagesTest) {
    function_guard mainGuard(this, {"main"}, {Type::get(SysYust::IR::Type::i)});
    entry_block();

    auto alc1 = auto_inst<alc>(Type::get(SysYust::IR::Type::i));
    auto alcName = alc1->assigned;
    alc1.push();

    auto opr1 = auto_inst<ld>(alcName);
    auto opr1Name = opr1->assigned;
    opr1.push();
    auto opr1DE = current_du().local_defined(opr1Name);

    im_symbol opr2(64);

    auto add1 = auto_inst<add>(opr1Name, opr2);
    auto add1Name = add1->assigned;
    add1.push();

    // 用例查询

    auto &add1DU = current_du().instDUInfo(add1.pushed_inst_handler());
    EXPECT_EQ(add1DU.inst, add1.pushed_inst_handler());
    auto &dep = add1DU.args;
    auto &&arg1 = *dep[0];
    auto &&arg2 = *dep[1];
    EXPECT_EQ(arg1.defined, opr1DE);
    EXPECT_EQ(arg2.defined, current_du().local_defined(opr2));

    // 用例修改

    auto nSym = nxt_sym();
    current_du().rename_define(opr1Name, nSym);
    opr1.pull();
    opr1Name = opr1->assigned;
    EXPECT_EQ(nSym, opr1Name);
    EXPECT_EQ(arg1.defined, current_du().local_defined(nSym));
    EXPECT_EQ(nSym, arg1.defined->defined.var()) << "new symbol: " << nSym.full() << std::endl
     << "argument symbol: " << arg1.defined->defined.var().full();

    // 语句修改

    /// @todo 语句修改

}