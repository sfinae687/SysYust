//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_AST_RETURN_H
#define SYSYUST_AST_RETURN_H

#include <utility>

#include "Stmt.h"

namespace SysYust::AST {

    class Return : public Stmt {
    public:
        explicit Return(std::size_t returned);

        void execute(SysYust::AST::NodeExecutor *e) override;

        std::size_t returned;
    };

} // AST

#endif //SYSYUST_AST_RETURN_H
