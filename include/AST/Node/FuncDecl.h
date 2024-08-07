//
// Created by LL06p on 24-7-5.
//

#ifndef SYSYUST_FUNCDECL_H
#define SYSYUST_FUNCDECL_H

#include <utility>
#include <memory>

#include "AST/Env.h"
#include "NodeBase.h"
#include "Decl.h"
#include "IdAllocator.h"

namespace SysYust::AST {

    class FuncDecl : public Decl {
    public:
        void execute(NodeExecutor *e) override;

        std::size_t entry_node = -1; ///< 类型一定是 Block
        NumId info_id = -1;
        std::vector<HNode> param{}; ///< ParamDecl s;
    };

} // AST

#endif //SYSYUST_FUNCDECL_H
