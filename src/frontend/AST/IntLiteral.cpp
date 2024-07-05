//
// Created by LL06p on 24-7-5.
//

#include "AST/IntLiteral.h"
#include "AST/NodeExecutor.h"

    namespace SysYust::AST {
        IntLiteral::IntLiteral()
        : Expr(&Int_v, true) {

        }

        IntLiteral::IntLiteral(std::int32_t v)
        : Expr(&Int_v, true)
        , value(v) {

        }

        void IntLiteral::execute(NodeExecutor *e) {
            e->execute(*this);
        }
    } // AST