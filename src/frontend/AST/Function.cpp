/// @file Function 类型标识的实现

#include "AST/Function.h"

namespace SysYust::AST {
    const Type &Function::getResult() const {
        return _returnedType;
    }

    std::vector<const Type *> Function::getParam() const {
        return _paramType;
    }

    bool Function::match(const Function &rhs) const {
        bool rtEqual = _returnedType.match(rhs._returnedType);
        bool paramEqual = std::equal(_paramType.begin(), _paramType.end(), rhs._paramType.begin());
        return rtEqual && paramEqual;
    }
} // AST