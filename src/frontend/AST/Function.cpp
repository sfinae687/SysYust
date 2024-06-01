/// @file Function 类型标识的实现

#include "AST/Function.h"

namespace SysYust::AST {
    const Type &Function::getResult() const {
        return _returnedType;
    }

    std::vector<const Type *> Function::getParam() const {
        return _paramType;
    }
} // AST