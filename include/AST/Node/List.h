//
// Created by LL06p on 24-7-11.
//

#ifndef SYSYUST_AST_LIST_H
#define SYSYUST_AST_LIST_H

#include <vector>
#include <span>

#include "NodeBase.h"

namespace SysYust::AST {

    class List : public Node {
    public:
        explicit List(std::span<const HNode> vals);
        std::vector<HNode> vals;
        void execute(SysYust::AST::NodeExecutor *e) override;
    };

} // AST

#endif //SYSYUST_AST_LIST_H
