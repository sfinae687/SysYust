//
// Created by LL06p on 24-7-8.
//

#include <gtest/gtest.h>

#include "AST/Type.h"

using namespace SysYust::AST;

TEST(TypeToStringTest, plain) {
    auto &t1 = Int_v;
    auto &t2 = Float_v;
    auto &t3 = Void_v;
    EXPECT_EQ(t1.toString(), "int");
    EXPECT_EQ(t2.toString(), "float");
    EXPECT_EQ(t3.toString(), "void");
}

TEST(TypeToStringTest, array) {
    auto &a1 = Array::create(Int_v, {1, 2, 3});
    EXPECT_EQ(a1.toString(), "int[1][2][3]");
}

TEST(TypeToStringTest, pointer) {
    auto &p1 = Pointer::create(Int_v);
    EXPECT_EQ(p1.toString(), "int*");
    auto &a = Array::create(Int_v, {1, 2, 3});
    auto &p2 = Pointer::create(a);
    EXPECT_EQ(p2.toString(), "int[1][2][3]*");
}

TEST(TypeToStringTest, function) {
    auto &f1 = Function::create(Int_v, Int_v);
    EXPECT_EQ(f1.toString(), "function(int) -> int");
    auto &f2 = Function::create(Int_v, Int_v, Float_v);
    EXPECT_EQ(f2.toString(), "function(int,float) -> int");
}