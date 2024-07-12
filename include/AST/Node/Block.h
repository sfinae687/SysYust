//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_BLOCK_H
#define SYSYUST_AST_BLOCK_H

#include <vector>

#include "Stmt.h"

namespace SysYust::AST {

    class Block : public Stmt {
    public:
        explicit Block(std::vector<HNode> stmts);

        void execute(SysYust::AST::NodeExecutor *e) override;

        std::vector<std::size_t> stmts;
    };

} // AST

#endif //SYSYUST_AST_BLOCK_H
