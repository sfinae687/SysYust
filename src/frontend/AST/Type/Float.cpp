//
// Created by LL06p on 24-5-31.
//

#include "AST/Type/Float.h"

namespace SysYust::AST {

    std::string Float::toString() const noexcept {
        return "float";
    }

    const Float Float_v;
} // AST