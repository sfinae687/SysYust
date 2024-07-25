//
// Created by LL06p on 24-7-20.
//

#include <stdexcept>

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <range/v3/action.hpp>

#include "utility/Logger.h"
#include "IR/BasicBlock.h"

namespace SysYust::IR {

    namespace ranges = ::ranges;
    namespace views = ::ranges::views;

    void BasicBlock::seal() {
        if (!std::visit(&instruct_base::is_gateway, back())) {
            LOG_WARN("Unable to seal a basic block without gateway");
        } else {
            sealed = true;
        }
    }

    bool BasicBlock::is_sealed() const {
        return sealed;
    }

    gateway_inst BasicBlock::gateway() const {
        if (is_sealed()) {
            auto &last_inst = back();
            return std::visit([](auto i) -> gateway_inst {
                    if constexpr (is_gateway(i.cateId)) {
                        return {i};
                    } else {
                        __builtin_unreachable();
                        return std::monostate{};
                    }
                }, last_inst);
        } else {
            LOG_ERROR("acquire a gateway for a unsealed basic block");
            throw std::domain_error("acquire a gateway for a unsealed basic block");
        }
    }

    void BasicBlock::setNext(BasicBlock *t) {
        assert(t);
        // 删除下一个节点的前驱
        if (trueTarget == nullptr) {
            t->_prevBlock.push_back(this);
        } else { // 原先有子节点
            auto target = ranges::find(t->_prevBlock, this);
            if (target != t->_prevBlock.end()) {
                t->_prevBlock.erase(target);
            }
        }
        trueTarget = t;
    }

    void BasicBlock::setElse(BasicBlock *t) {
        assert(t);
        // 删除下一个节点的前驱
        if (trueTarget == nullptr) {
            t->_prevBlock.push_back(this);
        } else { // 原先有子节点
            auto target = ranges::find(t->_prevBlock, this);
            if (target != t->_prevBlock.end()) {
                t->_prevBlock.erase(target);
            }
        }
        falseTarget = t;
    }

    BasicBlock *BasicBlock::nextBlock() {
        return trueTarget;
    }

    BasicBlock *BasicBlock::elseBlock() {
        return falseTarget;
    }

    void BasicBlock::push(instruction item) {
        if constexpr (strict_check_flag) {
            if (is_sealed()) {
                return ;
            }
        }
        instruction_list.emplace_back(std::move(item));
    }

    void BasicBlock::pop() {
        if constexpr (strict_check_flag) {
            if (is_sealed()) {
                return ;
            }
        }
        instruction_list.pop_back();
    }

    instruction &BasicBlock::back() {
        return instruction_list.back();
    }

    const instruction &BasicBlock::back() const {
        return instruction_list.back();
    }

    BasicBlock::iterator BasicBlock::erase(BasicBlock::iterator pos) {
        return instruction_list.erase(pos);
    }

    BasicBlock::iterator BasicBlock::erase(BasicBlock::iterator first, BasicBlock::iterator last) {
        return instruction_list.erase(first, last);
    }

    const std::vector<BasicBlock *> &BasicBlock::prevBlocks() {
        return _prevBlock;
    }

    BasicBlock::iterator BasicBlock::insert(BasicBlock::iterator pos, const instruction &value) {
        return instruction_list.insert(pos, value);
    }

    void BasicBlock::setArg(const std::vector<var_symbol>& names) {
        _block_parm.clear();
        _block_parm.insert(names.begin(), names.end());
    }

    const BasicBlock::block_arg_list_type & BasicBlock::getArg() const {
        return _block_parm;
    }

    bool BasicBlock::is_arg(const var_symbol& name) {
        return _block_parm.contains(name);
    }
} // IR