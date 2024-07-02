/// @file Type 类型标识的构造和 match 相等性比较的测试

#include <gtest/gtest.h>

#include "AST/Type.h"

using namespace SysYust::AST;

TEST(TypeMatchTest, PlainType) {
    EXPECT_TRUE(match(Int_v, Int_v)) << "Int match error";
    EXPECT_TRUE(match(Float_v, Float_v)) << "Float match error";
    EXPECT_TRUE(match(Void_v, Void_v)) << "Void match error";
}

TEST(TypeMatchTest, Array) {
    auto arr0 = Array(Int_v, {1, 2, 3});
    EXPECT_EQ(arr0.getRank(), 3) << "Uncorrected rank";
    EXPECT_EQ(arr0.getExtent(0), 1) << "Uncorrected order";
    auto arr1 = Array(Int_v, {1, 2, 3});
    auto arr2 = Array(Int_v, {2, 2, 3});
    auto arr3 = Array(Float_v, {1, 2, 3});
    EXPECT_TRUE(match(arr0, arr1)) << "Array match error";
    EXPECT_FALSE(match(arr0, arr2)) << "Array dimensions match error";
    EXPECT_FALSE(match(arr0, arr3)) << "Array base type match error";
}

TEST(TypeMatchTest, Pointer) {
    auto ptr0 = Pointer(Int_v);
    auto ptr1 = Pointer(Int_v);
    auto ptr2 = Pointer(Float_v);
    EXPECT_TRUE(match(ptr0, ptr1)) << "Pointer match error";
    EXPECT_FALSE(match(ptr0, ptr2)) << "Pointer match error";
    auto arr0 = Array(Int_v, {10});
    auto ptr3 = Pointer(Int_v);
    EXPECT_TRUE(match(ptr3, arr0)) << "Pointer-Array match error";
    EXPECT_TRUE(match(arr0, ptr3)) << "Pointer-Array match error";
}

TEST(TypeMatchTest, Function) {
    auto func0 = Function(Int_v, {&Int_v, &Int_v});
    auto func1 = Function(Int_v, Int_v, Int_v);
    const Type &rt = Int_v;
    auto func2 = Function(rt, {&Int_v, &Int_v});
    EXPECT_TRUE(match(func0, func1));
    EXPECT_TRUE(match(func1,func2));
    EXPECT_TRUE(match(func0, func2));
    EXPECT_TRUE(match(func0.getResult(), Int_v)) << "Unable to get correct return-type";
}