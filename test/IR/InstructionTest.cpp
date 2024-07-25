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
    auto sub1 = inst<sub>(var_symbol{"t", 2}, add1.assigned, im_symbol{122});
    auto mul1 = inst<mul>(var_symbol{"t", 3}, add1.assigned, sub1.assigned);
    auto div1 = inst<divi>(var_symbol{"t", 5, }, mul1.assigned, mul1.assigned);
    auto neg1 = inst<neg>(var_symbol{"t", 4}, mul1.assigned);
    auto mod1 = inst<mod>(var_symbol{"t", 6}, mul1.assigned, mul1.assigned);
    auto rem1 = inst<rem>(var_symbol{"t", 7}, mod1.assigned, mod1.assigned);
    auto i2b1 = inst<i2b>(var_symbol{"t", 9}, rem1.assigned);
    auto eq1 = inst<eq>(var_symbol{"t", 10}, i2b1.assigned, i2b1.assigned);
    auto ne1 = inst<ne>(var_symbol{"t", 11}, eq1.assigned, eq1.assigned);
    auto lt1 = inst<lt>(var_symbol{"t", 11}, ne1.assigned, ne1.assigned);
    auto le1 = inst<le>(var_symbol{"t", 12}, lt1.assigned, lt1.assigned);
    auto gt1 = inst<gt>(var_symbol{"t", 13}, le1.assigned, le1.assigned);
    auto ge1 = inst<ge>(var_symbol{"t", 0}, gt1.assigned, gt1.assigned);
    auto indexof1 = inst<indexof>(var_symbol{"t", 15}, mem2.assigned, std::vector<operant>{im_symbol{12}});

    auto i2f1 = inst<i2f>(var_symbol{"t", 8}, ge1.assigned);
    auto fneg1 = inst<fneg>(var_symbol{"t", 16}, i2f1.assigned);
    auto fadd1 = inst<fadd>(var_symbol{"t", 17}, fneg1.assigned, fneg1.assigned);
    auto fsub1 = inst<fsub>(var_symbol{"t", 18}, fadd1.assigned, fadd1.assigned);
    auto fmul1 = inst<fmul>(var_symbol{"t", 19}, fsub1.assigned, fsub1.assigned);
    auto frem1 = inst<frem>(var_symbol{"t", 20}, fmul1.assigned, fmul1.assigned);
    auto fdiv1 = inst<fdiv>(var_symbol{"t", 21}, frem1.assigned, frem1.assigned);
    auto f2i1 = inst<f2i>(var_symbol{"t", 22}, fdiv1.assigned);
    auto f2b1 = inst<f2b>(var_symbol{"t", 23}, fdiv1.assigned);
    auto feq1 = inst<feq>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    auto fne1 = inst<fne>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    auto flt1 = inst<flt>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    auto fle1 = inst<fle>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    auto fgt1 = inst<fgt>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);
    auto fge1 = inst<fge>(var_symbol{"t", 24}, fdiv1.assigned, fdiv1.assigned);

    auto call1 = inst<call>(var_symbol{"t", 25}, func_symbol{"hello"}, arg_list{});

    auto br1 = inst<br>(fgt1.assigned, arg_list{}, arg_list{});
    auto jp1 = inst<jp>(arg_list{});
    auto rt1 = inst<rt>();

}