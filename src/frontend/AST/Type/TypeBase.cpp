//
// Created by LL06p on 24-5-31.
//

#include "AST/Type/TypeBase.h"

namespace SysYust::AST {
    bool match(const Type &lhs, const Type &rhs) {
        return lhs.match(rhs);
    }
} // AST