//
// Created by LL06p on 24-7-5.
//

#include "AST/Node/BinaryOp.h"
#include "AST/Node/NodeExecutor.h"

namespace SysYust {
    namespace AST {
        BinaryOp::BinaryOp(Type *e_type, BinaryOp::OpType opt, std::size_t lhs, std::size_t rhs)
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