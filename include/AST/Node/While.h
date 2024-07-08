//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_WHILE_H
#define SYSYUST_AST_WHILE_H

#include <utility>

#include "Stmt.h"

namespace SysYust::AST {

    class While : public Stmt {
    public:
        While(HNode cond, HNode stmt);

        void execute(SysYust::AST::NodeExecutor *e) override;

        std::size_t cond;
        std::size_t stmt;
    };

} // AST

#endif //SYSYUST_AST_WHILE_H
