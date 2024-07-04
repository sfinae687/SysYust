/// @file Pointer 类型标识的实现

#include <cassert>

#include "AST/Type/Pointer.h"

namespace SysYust::AST {
    Pointer::Pointer(const Type &baseType)
    : _baseType(baseType) {
        assert(baseType.type() == TypeId::Int || baseType.type() == TypeId::Float || baseType.type() == TypeId::Array);
    }

    bool Pointer::match(const Array &rhs) const {
        return rhs.match(*this); // 将 match 操作委托给 Array 中定义的 match。
    }

    bool Pointer::match(const Pointer &rhs) const {
        return _baseType.match(rhs._baseType);
    }

    const Type &Pointer::getBase() const {
        return _baseType;
    }

    std::set<Pointer> Pointer::_pool{};
} // AST