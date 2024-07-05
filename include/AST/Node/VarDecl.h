//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_VARDECL_H
#define SYSYUST_AST_VARDECL_H

#include "utility/IdAllocator.h"
#include "NodeBase.h"
#include "Decl.h"

namespace SysYust::AST {

    class VarDecl : public Decl{
    public:
        void execute(NodeExecutor *e) override;

        NumId info_id = -1;
    };

} // AST

#endif //SYSYUST_AST_VARDECL_H
