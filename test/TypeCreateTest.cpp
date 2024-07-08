/// @file 测试类型标识的的创建行为

#include <gtest/gtest.h>

#include <AST/Type.h>

namespace AST = SysYust::AST;

TEST(TypeCreateTest, PlainTypeTest) {
    const auto &i = AST::Int_v;
    const auto &f = AST::Float_v;
    const auto &v = AST::Void_v;
}

TEST(TypeCreateTest, ArrayTest) {
    auto &a1 = AST::Array::create(AST::Int_v, {1, 2, 3});
    auto &a2 = AST::Array::create(AST::Int_v, {1, 2, 3});
    auto &a3 = AST::Array::create(AST::Float_v, {1, 2, 3});
    auto &a4 = AST::Array::create(AST::Int_v, {1, 2});
    EXPECT_EQ(&a1, &a2) << "Array with same argument don't have same address";
    EXPECT_NE(&a1, &a3) << "Array with different argument have same address";
    EXPECT_NE(&a1, &a4) << "Array with different base type have same address";

    auto &a5 = AST::Array::create(a4, {3});
    EXPECT_EQ(&a1, &a5) << "Array constructed by Array is out of excepted";
}

// 并不单元的单元测试
TEST(TypeCreateTest, PointerTest) {
    auto &p1 = AST::Pointer::create(AST::Int_v);
    auto &p2 = AST::Pointer::create(AST::Int_v);
    EXPECT_EQ(&p1, &p2) << "Pointer pointed to same type haven't same address";
    auto &p3 = AST::Pointer::create(AST::Float_v);
    EXPECT_NE(&p1, &p3) << "Pointer pointed to different type have same address";

    auto &a1 = AST::Array::create(AST::Int_v, {1, 2, 3});
    auto &a2 = AST::Array::create(AST::Int_v, {1, 2, 3});
    auto &a3 = AST::Array::create(AST::Float_v, {1, 2, 3});
    auto &p4 = AST::Pointer::create(a1);
    auto &p5 = AST::Pointer::create(a2);
    EXPECT_EQ(&p4, &p5);
    auto &p6 = AST::Pointer::create(a3);
    EXPECT_NE(&p4, &p6);

}

TEST(TypeCreateTest, FunctionTest) {
    auto &f1 = AST::Function::create(AST::Int_v, {&AST::Void_v});
    auto &f2 = AST::Function::create(AST::Int_v, AST::Void_v);
    EXPECT_EQ(&f1, &f2) << "Same Function type haven't same address";
}