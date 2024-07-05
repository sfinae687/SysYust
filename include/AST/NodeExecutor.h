//
// Created by LL06p on 24-7-4.
//

#ifndef SYSYUST_AST_NODEEXECUTOR_H
#define SYSYUST_AST_NODEEXECUTOR_H

#include "NodeBase.h"

namespace SysYust::AST {

    class NodeExecutor {
    public:
        virtual void execute(const VarDecl&) = 0;
        virtual void execute(const FuncDecl&) = 0;
        virtual void execute(const ParamDecl&) = 0;
        virtual void execute(const IntLiteral&) = 0;
        virtual void execute(const FloatLiteral&) = 0;
        virtual void execute(const Call&) = 0;
        virtual void execute(const UnaryOp&) = 0;
        virtual void execute(const BinaryOp&) = 0;
        virtual void execute(const DeclRef&) = 0;
        virtual void execute(const ArrayRef&) = 0;
        virtual void execute(const ToInt&) = 0;
        virtual void execute(const ToFloat&) = 0;
        virtual void execute(const Not&) = 0;
        virtual void execute(const And&) = 0;
        virtual void execute(const Or&) = 0;
        virtual void execute(const Compare&) = 0;
        virtual void execute(const ToCond&) = 0;
        virtual void execute(const If&) = 0;
        virtual void execute(const Assign&) = 0;
        virtual void execute(const While&) = 0;
        virtual void execute(const DoWhile&) = 0;
        virtual void execute(const For&) = 0;
        virtual void execute(const Break&) = 0;
        virtual void execute(const Continue&) = 0;
        virtual void execute(const Return&) = 0;
        virtual void execute(const Block&) = 0;
        virtual void execute(const Empty&) = 0;
    };

    class NodeExecutorBase : public NodeExecutor {
    public:
        void execute(const VarDecl&) override {}
        void execute(const FuncDecl&) override {}
        void execute(const ParamDecl&) override {}
        void execute(const IntLiteral&) override {}
        void execute(const FloatLiteral&) override {}
        void execute(const Call&) override {}
        void execute(const UnaryOp&) override {}
        void execute(const BinaryOp&) override {}
        void execute(const DeclRef&) override {}
        void execute(const ArrayRef&) override {}
        void execute(const ToInt&) override {}
        void execute(const ToFloat&) override {}
        void execute(const Not&) override {}
        void execute(const And&) override {}
        void execute(const Or&) override {}
        void execute(const Compare&) override {}
        void execute(const ToCond&) override {}
        void execute(const If&) override {}
        void execute(const Assign&) override {}
        void execute(const While&) override {}
        void execute(const DoWhile&) override {}
        void execute(const For&) override {}
        void execute(const Break&) override {}
        void execute(const Continue&) override {}
        void execute(const Return&) override {}
        void execute(const Block&) override {}
        void execute(const Empty&) override {}
    };

} // AST

#endif //SYSYUST_AST_NODEEXECUTOR_H
