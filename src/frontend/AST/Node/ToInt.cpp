//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/ToInt.h"
#include "AST/Node/NodeExecutor.h"

    namespace SysYust::AST {
        ToInt::ToInt(std::size_t operant)
        : Expr(&Int_v, false)
        , operant(operant) {

        }

        void ToInt::execute(NodeExecutor *e) {
            e->execute(*this);
        }
    } // AST
// SysYust