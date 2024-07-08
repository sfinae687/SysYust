//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_ASSIGN_H
#define SYSYUST_AST_ASSIGN_H

#include <utility>

#include "Stmt.h"

namespace SysYust::AST {

    class Assign : public Stmt {
    public:
        Assign(HNode l_val, HNode r_val);

        void execute(SysYust::AST::NodeExecutor *e) override;

        std::size_t l_val;
        std::size_t r_val;

    };

} // AST

#endif //SYSYUST_AST_ASSIGN_H
