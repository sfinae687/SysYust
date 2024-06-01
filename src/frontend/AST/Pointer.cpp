/// @file Pointer 类型标识的实现

#include "AST/Pointer.h"

namespace SysYust::AST {
    bool Pointer::match(const Array &rhs) const {
        return rhs == *this;
    }

    bool Pointer::match(const Pointer &rhs) const {
        return _baseType == rhs;
    }

    const Type &Pointer::getBase() const {
        return _baseType;
    }

} // AST