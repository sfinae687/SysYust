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

    std::size_t BasicBlock::_last_id = 0;

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

    /**
     * todo 重写gateway并加入带有上下文的gateway
     */
    gateway_inst BasicBlock::gateway() const {
        auto &last_inst = back();
        return std::visit([](auto i) -> gateway_inst {
                if constexpr (is_gateway(i.cateId)) {
                    return {i};
                } else {
                    __builtin_unreachable();
                    return std::monostate{};
                }
            }, last_inst);
    }

    void BasicBlock::setNext(BasicBlock *t) {
        assert(t);
        // 处理前驱
        if (trueTarget != nullptr) { // 原先有子节点
            auto toErase = ranges::find(trueTarget->_prevBlock, this);
            if (toErase != trueTarget->_prevBlock.end()) {
                trueTarget->_prevBlock.erase(toErase);
            }
        }
        t->_prevBlock.push_back(this);
        trueTarget = t;
    }

    void BasicBlock::setElse(BasicBlock *t) {
        assert(t);
        // 删除下一个节点的前驱
        if (falseTarget != nullptr) { // 原先有子节点
            auto target = ranges::find(falseTarget->_prevBlock, this);
            if (target != falseTarget->_prevBlock.end()) {
                falseTarget->_prevBlock.erase(target);
            }
        }
        t->_prevBlock.push_back(this);
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

    void BasicBlock::setArgs(const std::vector<var_symbol>& names) {
        _block_parm = names;
    }

    const BasicBlock::block_arg_list_type & BasicBlock::getArgs() const {
        return _block_parm;
    }

    bool BasicBlock::is_arg(const var_symbol& name) {
        auto founded = ranges::find(_block_parm, name);
        return founded != _block_parm.end();
    }

    void BasicBlock::add_arg(var_symbol name) {
        _block_parm.emplace_back(std::move(name));
    }

    bool BasicBlock::remove_arg(var_symbol name) {
        auto founded = ranges::find(_block_parm, name);
        if (founded != _block_parm.end()) {
            _block_parm.erase(founded);
            return true;
        } else {
            return false;
        }
    }

    void BasicBlock::remove_arg(std::size_t ind) {
        _block_parm.erase(_block_parm.begin() + ind);
    }

    std::size_t BasicBlock::arg_index(var_symbol name) const {
        auto founded = ranges::find(_block_parm, name);
        if (founded != _block_parm.end()) {
            return founded - _block_parm.begin();
        } else {
            return -1;
        }
    }
} // IR
