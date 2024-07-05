//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_BREAK_H
#define SYSYUST_AST_BREAK_H

#include "Stmt.h"

namespace SysYust::AST {

    class Break : public Stmt {
    public:
        void execute(SysYust::AST::NodeExecutor *e) override;
    };

} // AST

#endif //SYSYUST_AST_BREAK_H
