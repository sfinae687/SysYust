//
// Created by LL06p on 24-7-20.
//

#include <algorithm>

#include "IR/ControlFlow.h"


namespace SysYust::IR {

    BasicBlock *ControlFlow::add() {
        _nodes.emplace_back();
        auto addr = std::addressof(_nodes.back());
        if (_nodes.size() == 1) {
            setHead(addr);
        }
        return addr;
    }

    bool ControlFlow::erase(BasicBlock *t) {
        auto it = std::find_if(_nodes.begin(), _nodes.end(), [t](auto &i) {
            return std::addressof(i) == t;
        });
        if (it != _nodes.end()) {
            if (t == _head) {
                setHead(nullptr);
            }
            _nodes.erase(it);
            return true;
        } else {
            return false;
        }
    }

    BasicBlock *ControlFlow::head() const {
        return _head;
    }

    void ControlFlow::setHead(BasicBlock *b) {
        this->_head = b;
    }

    const std::list<BasicBlock> &ControlFlow::all_nodes() const {
        return _nodes;
    }
} // IR
