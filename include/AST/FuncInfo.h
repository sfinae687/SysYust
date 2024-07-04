/// @file 函数符号信息

#ifndef SYSYUST_AST_FUNCINFO_H
#define SYSYUST_AST_FUNCINFO_H

#include <string>
#include <memory>

#include "Type.h"

namespace SysYust::AST {

    class Decl;

    /**
     * @brief 函数条目信息
     */
    struct FuncInfo {
        std::string name;
        const Function *type;
        std::weak_ptr<Decl> node;
    };

} // AST

#endif //SYSYUST_AST_FUNCINFO_H
