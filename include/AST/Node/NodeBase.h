/// @file 节点相关设施

#ifndef SYSYUST_AST_NODEBASE_H
#define SYSYUST_AST_NODEBASE_H

#include <utility>
#include <string>

namespace SysYust::AST {

    // 节点（的引用）在树中保存在顺序容器中，按照索引相互引用。

    class Node;

    class Decl;
    class VarDecl;
    class FuncDecl;
    class ParamDecl;

    class List;

    class Stmt;
        class Expr;
            class IntLiteral;
            class FloatLiteral;
            class Call;
            class UnaryOp;
            class BinaryOp;
            class LExpr;
                class DeclRef;
                class ArrayRef;
            class ToInt;
            class ToFloat;
        class CondExpr;
            class Not;
            class And;
            class Or;
            class Compare;
            class ToCond;
        class If;
        class Assign;
        class While;
        class Break;
        class Continue;
        class Return;
        class Empty;
        class Block;


    class NodeExecutor;

    using HNode = std::size_t;

    /**
     * @brief 实现节点基类
     */
    class Node {
    public:
        virtual ~Node() = default;

        /**
         * @brief 访问器模式方法
         */
        virtual void execute(NodeExecutor *e) = 0;
    };

} // AST

#endif //SYSYUST_AST_NODEBASE_H
