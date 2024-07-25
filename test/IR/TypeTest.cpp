//
// Created by LL06p on 24-7-25.
//
#include <gtest/gtest.h>

#include "IR/TypeUtil.h"

namespace IR = SysYust::IR;

TEST(IRTest, TypeCreate) {
    auto it = IR::Type::get(SysYust::IR::Type::i);
    auto it2 = IR::Type::get(SysYust::IR::Type::i);
    EXPECT_EQ(it, it2) << "Expected equal address between two integer type identical";
    EXPECT_EQ(it->id(), IR::Type::i);
    EXPECT_TRUE(it->isInt());
    EXPECT_TRUE(it->isBasic());
    EXPECT_TRUE(it->isScalar());

    auto ft = IR::Type::get(SysYust::IR::Type::f);
    auto ft2 = IR::Type::get(SysYust::IR::Type::f);
    EXPECT_NE(it, ft);
    EXPECT_EQ(ft, ft2);
    EXPECT_EQ(ft->id(), IR::Type::f);
    EXPECT_TRUE(ft->isFlt());

    auto a1 = IR::Type::get(SysYust::IR::Type::arr, it, 12);
    auto a2 = IR::Type::get(SysYust::IR::Type::arr, IR::Type::get(SysYust::IR::Type::i), 12);
    auto a3 = IR::Type::get(SysYust::IR::Type::arr, it, 13);
    EXPECT_EQ(a1, a2);
    EXPECT_EQ(a1->root_type(), a2->root_type());
    EXPECT_NE(a1, a3);

    auto p = IR::Type::get(SysYust::IR::Type::ptr, it);
    auto p2 = IR::Type::get(SysYust::IR::Type::ptr, IR::Type::get(SysYust::IR::Type::i));
    EXPECT_EQ(p, p2);

}

TEST(IRTest, TypeSizeTest) {
    auto i1 = IR::Type::get(SysYust::IR::Type::i);
    auto f1 = IR::Type::get(SysYust::IR::Type::f);
    auto pi = IR::Type::get(SysYust::IR::Type::ptr, i1);
    auto pf = IR::Type::get(SysYust::IR::Type::ptr, f1);
    EXPECT_EQ(i1->size(), 4);
    EXPECT_EQ(f1->size(), 4);
    EXPECT_EQ(pi->size(), 8);
    EXPECT_EQ(pf->size(), 8);
    EXPECT_EQ(pf->size(), pi->size());

    auto a1 = IR::Type::get(SysYust::IR::Type::arr, i1, 8);
    auto a2 = IR::Type::get(SysYust::IR::Type::arr, i1, 16);
    auto a3 = IR::Type::get(SysYust::IR::Type::arr, a1, 8);
    EXPECT_EQ(a1->size(), i1->size() * 8);
    EXPECT_EQ(a2->size(), i1->size() * 16);
    EXPECT_EQ(a3->size(), i1->size() * 8 * 8);
}
