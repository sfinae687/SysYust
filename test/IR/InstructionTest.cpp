//
// Created by LL06p on 24-7-25.
//
#include <gtest/gtest.h>

#include "IR/Instruction.h"

using namespace SysYust::IR;

TEST(IRTest, InstructionCreate) {

    // 检测这些类型的构造是否正常以及类型传播是否正常

    auto mem1 = inst<alc>(var_symbol{"t", 0}, Type::get(SysYust::IR::Type::i));
    auto mem2 = inst<alc>(var_symbol{"a", 0}, Type::get(SysYust::IR::Type::arr, Type::get(SysYust::IR::Type::i), 64));
    auto st1 = inst<st>(mem1.assigned, im_symbol{115514});
    auto ld1 = inst<ld>(var_symbol{"t", 1}, mem1.assigned);

    auto add1 = inst<add>(var_symbol{"t", 1}, im_symbol{124}, im_symbol{128});
    EXPECT_TRUE(add1.assigned.type->isInt());
    auto sub1 = inst<sub>(var_symbol{"t", 2}, add1.assigned, im_symbol{122});
    EXPECT_TRUE(sub1.assigned.type->isInt());
    auto mul1 = inst<mul>(var_symbol{"t", 3}, add1.assigned, sub1.assigned);
    EXPECT_TRUE(mul1.assigned.type->isInt());
    auto div1 = inst<divi>(var_symbol{"t", 5, }, mul1.assigned, mul1.assigned);
    EXPECT_TRUE(div1.assigned.type->isInt());
    auto neg1 = inst<neg>(var_symbol{"t", 4}, mul1.assigned);
    EXPECT_TRUE(neg1.assigned.type->isInt());
    auto mod1 = inst<mod>(var_symbol{"t", 6}, mul1.assigned, mul1.assigned);
    EXPECT_TRUE(mod1.assigned.type->isInt());
    auto rem1 = inst<rem>(var_symbol{"t", 7}, mod1.assigned, mod1.assigned);
    EXPECT_TRUE(rem1.assigned.type->isInt());
    auto i2b1 = inst<i2b>(var_symbol{"t", 9}, rem1.assigned);
    EXPECT_TRUE(i2b1.assigned.type->isInt());
    auto eq1 = inst<eq>(var_symbol{"t", 10}, i2b1.assigned, i2b1.assigned);
    EXPECT_TRUE(eq1.assigned.type->isInt());
    auto ne1 = inst<ne>(var_symbol{"t", 11}, eq1.assigned, eq1.assigned);
    EXPECT_TRUE(ne1.assigned.type->isInt());
    auto lt1 = inst<lt>(var_symbol{"t", 11}, ne1.assigned, ne1.assigned);
    EXPECT_TRUE(lt1.assigned.type->isInt());
    auto le1 = inst<le>(var_symbol{"t", 12}, lt1.assigned, lt1.assigned);
    EXPECT_TRUE(le1.assigned.type->isInt());
    auto gt1 = inst<gt>(var_symbol{"t", 13}, le1.assigned, le1.assigned);
    EXPECT_TRUE(gt1.assigned.type->isInt());
    auto ge1 = inst<ge>(var_symbol{"t", 0}, gt1.assigned, gt1.assigned);
    EXPECT_TRUE(ge1.assigned.type->isInt());
    auto indexof1 = inst<indexof>(var_symbol{"t", 15}, mem2.assigned, std::vector<operant>{im_symbol{12}});
    EXPECT_TRUE(indexof1.assigned.type->isPtr());

    auto i2f1 = inst<i2f>(var_symbol{"t", 8}, ge1.assigned);
    EXPECT_TRUE(i2f1.assigned.type->isFlt());
    auto fneg1 = inst<fneg>(var_symbol{"t", 16}, i2f1.assigned);
    EXPECT_TRUE(fneg1.assigned.type->isFlt());
    auto fadd1 = inst<fadd>(var_symbol{"t", 17}, fneg1.assigned, fneg1.assigned);
    EXPECT_TRUE(fadd1.assigned.type->isFlt());
    auto fsub1 = inst<fsub>(var_symbol{"t", 18}, fadd1.assigned, fadd1.assigned);
    EXPECT_TRUE(fsub1.assigned.type->isFlt());
    auto fmul1 = inst<fmul>(var_symbol{"t", 19}, fsub1.assigned, fsub1.assigned);
    EXPECT_TRUE(fmul1.assigned.type->isFlt());
    auto frem1 = inst<frem>(var_symbol{"t", 20}, fmul1.assigned, fmul1.assigned);
    EXPECT_TRUE(frem1.assigned.type->isFlt());
    auto fdiv1 = inst<fdiv>(var_symbol{"t", 21}, frem1.assigned, frem1.assigned);
    EXPECT_TRUE(fdiv1.assigned.type->isFlt());
    auto f2i1 = inst<f2i>(var_symbol{"t", 22}, fdiv1.assigned);
    EXPECT_TRUE(f2i1.assigned.type->isInt());
    auto f2b1 = inst<f2b>(var_symbol{"t", 23}, fdiv1.assigned);
    EXPECT_TRUE(f2b1.assigned.type->isInt());
    auto feq1 = inst<feq>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    EXPECT_TRUE(feq1.assigned.type->isInt());
    auto fne1 = inst<fne>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    EXPECT_TRUE(fne1.assigned.type->isInt());
    auto flt1 = inst<flt>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    EXPECT_TRUE(flt1.assigned.type->isInt());
    auto fle1 = inst<fle>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    EXPECT_TRUE(fle1.assigned.type->isInt());
    auto fgt1 = inst<fgt>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    EXPECT_TRUE(fgt1.assigned.type->isInt());
    auto fge1 = inst<fge>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    EXPECT_TRUE(fge1.assigned.type->isInt());

    auto call1 = inst<call>(var_symbol{"t", 25}, func_symbol{"hello"}, arg_list{});

    auto br1 = inst<br>(fgt1.assigned, arg_list{}, arg_list{});
    auto jp1 = inst<jp>(arg_list{});
    auto rt1 = inst<rt>();

}