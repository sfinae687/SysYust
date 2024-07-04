/// @file 变量的符号信息条目

#ifndef SYSYUST_VARINFO_H
#define SYSYUST_VARINFO_H

#include <string>
#include <memory>

#include "Type.h"

namespace SysYust::AST {

    class Decl;

    /**
     * @brief 变量符号信息条目
     */
    struct VarInfo {
        std::string name;
        const Type *type;
        bool isConstant;
    };

} // AST

#endif //SYSYUST_VARINFO_H
