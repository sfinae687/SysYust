/// @file 函数符号信息

#ifndef SYSYUST_AST_FUNCINFO_H
#define SYSYUST_AST_FUNCINFO_H

#include <string>
#include <memory>

#include "AST/Type.h"

namespace SysYust::AST {

    class FuncDecl;

    /**
     * @brief 函数条目信息
     */
    struct FuncInfo {
        std::string name;
        const Function *type;
        std::size_t node; ///< 声明它的节点的索引编号
    };

} // AST

#endif //SYSYUST_AST_FUNCINFO_H
