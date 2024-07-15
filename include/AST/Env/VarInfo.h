/// @file 变量的符号信息条目

#ifndef SYSYUST_AST_VARINFO_H
#define SYSYUST_AST_VARINFO_H

#include <string>
#include <memory>

#include "AST/Type.h"

namespace SysYust::AST {

    using HNode = std::size_t;

    /**
     * @brief 变量符号信息条目
     */
    struct VarInfo {
        std::string name;
        const Type *type;
        HNode decl;
        bool isConstant;
        bool isParam;
    };

} // AST

#endif //SYSYUST_AST_VARINFO_H
