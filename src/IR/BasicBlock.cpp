//
// Created by LL06p on 24-7-20.
//

#include <stdexcept>

#include "utility/Logger.h"
#include "IR/BasicBlock.h"

namespace SysYust::IR {
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

    void BasicBlock::setTarget(BasicBlock *t) {
        trueTarget = t;
    }

    void BasicBlock::setElseTarget(BasicBlock *t) {
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

    BasicBlock::iterator BasicBlock::insert(BasicBlock::iterator pos,
                                            const std::variant<compute_with_2, compute_with_1, indexOf, load, store, alloc, call_instruct, branch, jump, ret> &value) {
        return instruction_list.insert(pos, value);
    }

    BasicBlock::iterator BasicBlock::erase(BasicBlock::iterator pos) {
        return instruction_list.erase(pos);
    }

    BasicBlock::iterator BasicBlock::erase(BasicBlock::iterator first, BasicBlock::iterator last) {
        return instruction_list.erase(first, last);
    }
} // IR