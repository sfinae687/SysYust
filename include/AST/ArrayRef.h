//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_ARRAYREF_H
#define SYSYUST_AST_ARRAYREF_H

#include "LExpr.h"
#include "IdAllocator.h"

namespace SysYust::AST {

    class ArrayRef : public LExpr{
    public:
        ArrayRef(Type *type, NumId id, std::vector<std::size_t> sub);

        void execute(NodeExecutor *e) override;

        NumId var_id;
        std::vector<std::size_t> subscripts;
    };

} // AST

#endif //SYSYUST_AST_ARRAYREF_H
