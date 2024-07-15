/// @file Void 类型标识的实现

#include "AST/Type/Void.h"

namespace SysYust::AST {
    const Void Void_v;

    std::string Void::toString() const noexcept {
        return "void";
    }
} // AST