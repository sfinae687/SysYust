//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_PARAMDECL_H
#define SYSYUST_AST_PARAMDECL_H

#include "NodeBase.h"
#include "Decl.h"
#include "utility/IdAllocator.h"

namespace SysYust::AST {

    class ParamDecl : public Decl {
    public:
        void execute(NodeExecutor *e) override;

        NumId info_id = -1;
    };

} // AST

#endif //SYSYUST_AST_PARAMDECL_H
