//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_VARDECL_H
#define SYSYUST_AST_VARDECL_H

#include <variant>
#include <vector>
#include <optional>

#include "utility/IdAllocator.h"
#include "NodeBase.h"
#include "Decl.h"

namespace SysYust::AST {

    class VarDecl : public Decl{
    public:
        void execute(NodeExecutor *e) override;

        NumId info_id = -1;
        std::optional<HNode> init_expr; //为 expr 或 list
    };

} // AST

#endif //SYSYUST_AST_VARDECL_H
