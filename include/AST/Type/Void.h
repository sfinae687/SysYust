/// @file 类型标识 Void 的定义

#ifndef SYSYUST_AST_VOID_H
#define SYSYUST_AST_VOID_H

#include "TypeBase.h"

    namespace SysYust::AST {

        class Void: public TypeBase<Void> {

        };

        extern const Void Void_v;

    } // AST

#endif //SYSYUST_AST_VOID_H
