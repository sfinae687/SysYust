//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/BinaryOp.h"
#include "AST/Node/NodeExecutor.h"

namespace SysYust {
    namespace AST {
        BinaryOp::BinaryOp(const Type *e_type, OpType opt, HNode lhs, HNode rhs)
        : Expr(e_type, false)
        , type(opt)
        , lhs(lhs)
        , rhs(rhs) {

        }

        void BinaryOp::execute(NodeExecutor *e) {
            e->execute(*this);
        }
    } // AST
} // SysYust