/// @file Int 类型标识定义

#ifndef SYSYUST_AST_INT_H
#define SYSYUST_AST_INT_H

#include "TypeBase.h"

namespace SysYust::AST {

    class Int: public TypeBase<Int> {

    };

    extern const Int Int_v;

} // AST

#endif //SYSYUST_AST_INT_H
