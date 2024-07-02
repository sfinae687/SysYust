/// @file Float 类型标识定义

#ifndef SYSYUST_AST_FLOAT_H
#define SYSYUST_AST_FLOAT_H

#include "TypeBase.h"

namespace SysYust::AST {

    class Float: public TypeBase<Float> {

    };

    extern const Float Float_v;

} // AST

#endif //SYSYUST_AST_FLOAT_H
