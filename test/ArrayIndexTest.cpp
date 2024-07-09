//
// Created by LL06p on 24-7-9.
//

#include <gtest/gtest.h>

#include "AST/Type.h"

using namespace SysYust::AST;

TEST(ArrayIndexTest, ArrayPlainTest) {
    auto &a1 = Array::create(Int_v, {10});
    EXPECT_EQ(&a1.index(0), &a1);
    EXPECT_EQ(&a1.index(1), &Int_v);

    std::vector<std::size_t> offset{5};
    EXPECT_EQ(a1.offsetWith(offset), 5);
    offset[0] = 0;
    EXPECT_EQ(a1.offsetWith(offset), 0);
}

TEST(ArrayIndexTest, ArrayTest) {
    // 相当于声明 int a1[4][20][10]
    auto &a1 = Array::create(Int_v, {10, 20, 4});

    auto &a2 = Array::create(Int_v, {10, 20});
    auto &a3 = Array::create(Int_v, {10});
    EXPECT_EQ(&a1.index(1), &a2) << a1.index(1).toString() << ":" << a2.toString();
    EXPECT_EQ(&a1.index(2), &a3) << a1.index(2).toString() << ":" << a3.toString();
    EXPECT_EQ(&a1.index(3), &Int_v);

    std::vector<std::size_t> offset1{1, 2, 3};
    std::vector<std::size_t> offset2{1, 2};
    std::vector<std::size_t> offset3{1};
    EXPECT_EQ(a1.offsetWith(offset1), 223);
    EXPECT_EQ(a1.offsetWith(offset2), 220);
    EXPECT_EQ(a1.offsetWith(offset3), 200);
}

TEST(ArrayIndexTest, PointerTest) {
    auto &p1 = Pointer::create(Int_v);
    EXPECT_EQ(&p1.index(0), &p1);
    EXPECT_EQ(&p1.index(1), &Int_v);

    std::vector<std::size_t> offset{0};
    EXPECT_EQ(p1.offsetWith(offset), 0);
    offset = {15};
    EXPECT_EQ(p1.offsetWith(offset), 15);

    auto &a1 = Array::create(Int_v, {10, 20, 10});
    auto &p2 = Pointer::create(a1);
    auto &a2 = Array::create(Int_v, {10, 20});
    auto &a3 = Array::create(Int_v, {10});
    EXPECT_EQ(&p2.index(0), &p2);
    EXPECT_EQ(&p2.index(1), &a1);
    EXPECT_EQ(&p2.index(2), &a2);
    EXPECT_EQ(&p2.index(4), &Int_v);
    offset = {1};
    EXPECT_EQ(p2.offsetWith(offset), 2000);
    offset = {1, 2};
    EXPECT_EQ(p2.offsetWith(offset), 2000+2*200);
    offset = {1, 2, 3};
    EXPECT_EQ(p2.offsetWith(offset), 2000+2*200+3*10);
}