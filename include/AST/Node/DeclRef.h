//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_DECLREF_H
#define SYSYUST_AST_DECLREF_H

#include "LExpr.h"
#include "IdAllocator.h"

namespace SysYust::AST {

    class DeclRef : public LExpr {
    public:
        explicit DeclRef(const Type *e_type, NumId id);

        void execute(NodeExecutor *e) override;

        NumId var_id = 0;
    };

} // AST

#endif //SYSYUST_AST_DECLREF_H
