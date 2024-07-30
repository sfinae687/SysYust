//
// Created by LL06p on 24-7-28.
//

#include <utility>

#include "IR/Def-Use/DU_Util.h"

namespace SysYust::IR {

    DefineEntry::DefineEntry(im_symbol im)
        : defined(im)
        , define_type(SymbolDefineType::builtin)
    {

    }

    DefineEntry::DefineEntry(var_symbol var, DUInst *du, SymbolDefineType type)
        : defined(std::move(var))
        , define_type([&] {
            if (type != SymbolDefineType::unknown) {
                return type;
            }
            if (du) {
                return SymbolDefineType::block;
            } else {
                return SymbolDefineType::func_param;
            }
        }())
        , inst_context(du)
    {

    }

    DefineEntry::DefineEntry(var_symbol var, BasicBlock *block)
        : defined(std::move(var))
        , define_type(SymbolDefineType::block_param)
        , inst_context(block)
    {

    }

    std::vector<usage_pointer> & DefineEntry::dependence() const {
        return std::get<DUInst*>(inst_context)->args;
    }

    BasicBlock *DefineEntry::block() const {
        if (auto id = inst_context.index(); id == 1) {
            return std::get<DUInst*>(inst_context)->block;
        } else if (id == 2) {
            return std::get<BasicBlock*>(inst_context);
        } else {
            return nullptr;
        }
    }

    std::optional<BasicBlock::iterator> DefineEntry::inst() const {
        if (inst_context.index() == 1) {
            return std::get<DUInst*>(inst_context)->inst;
        } else {
            return std::nullopt;
        }
    }

    DUInst *DefineEntry::du_inst() const {
        return std::get<DUInst*>(inst_context);
    }

    void UsageEntry::setOpr(const operant& nOpr) const {
        set_arg_at(*inst, index, nOpr);
    }

    operant UsageEntry::opr() const {
        return arg_at(*inst, index);
    }
}