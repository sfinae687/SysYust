/// @file Pointer 类型标识的实现

#include "AST/Pointer.h"

namespace SysYust::AST {
    bool Pointer::match(const Array &rhs) const {
        return rhs.match(*this); // 将 match 操作委托给 Array 中定义的 match。
    }

    bool Pointer::match(const Pointer &rhs) const {
        return _baseType.match(rhs._baseType);
    }

    const Type &Pointer::getBase() const {
        return _baseType;
    }

} // AST