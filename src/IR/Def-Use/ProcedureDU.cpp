//
// Created by LL06p on 24-7-28.
//

#include <range/v3/algorithm.hpp>

#include "utility/Logger.h"
#include "IR/Procedure.h"
#include "IR/Def-Use/ProcedureDU.h"

namespace SysYust::IR {


    ProcedureDU::ProcedureDU(Procedure &context)
        : global(context.context()->global)
        , procedure(context)
    {
        for (auto &i : procedure.context()->param_var_set) {
            add_func_param(i);
        }
    }

    void ProcedureDU::add_inst(BasicBlock *block, BasicBlock::iterator inst) {

        if (static_cast<instruct_cate>(inst->index()) == instruct_cate::rv_inst) {
            auto &&rvi = std::get<RiscVInstruction>(*inst);
            if (rvi.register_arg) {
                return;
            }
        }

        // 一条语句对定义-依赖的影响：
        //  添加一条你定义符号
        //  添加若干条依赖使用

        std::optional<var_symbol> defined;
        std::vector<usage_pointer> usages{};

        // 获取定义符号名和用例符号名

        defined = std::visit(get_assigned, *inst);

        auto arg_c = arg_size(*inst);

        for (std::size_t i = 0; i<arg_c; ++i) {
            usages.push_back(add_usage(block, inst, arg_at(*inst, i), i));
        }

        // 创建定义条目和语句条目

        auto &du_inst = _inst_list[&*inst] = {block, inst, std::move(usages), nullptr};
        if (defined && *defined != procedure.context()->depressed_symbol) {

#ifndef NEDEBUG
            auto founded = _define_list.find(*defined);
            if (founded != _define_list.end()) {
                LOG_ERROR("define a symbol that has defined : defined {}, to define {}", founded->first.var().full(), defined->full());
            }
#endif

            auto [define_iter, _] = _define_list.emplace(std::piecewise_construct,
                                                      std::tuple{*defined}, std::tuple{*defined, &du_inst});
            du_inst.defineInfo = &define_iter->second;
        }

    }

    bool ProcedureDU::remove_inst(BasicBlock::iterator inst) {

        if (static_cast<instruct_cate>(inst->index()) == instruct_cate::rv_inst) {
            auto &&rvi = std::get<RiscVInstruction>(*inst);
            if (rvi.register_arg) {
                return true;
            }
        }

        std::optional<value_type> defined;
        defined = std::visit(get_assigned, *inst);

        // 移除定义条目
        if (defined && *defined != procedure.context()->depressed_symbol) {
            // 仅当无后续用例移除定义条目
            if (!remove_define(*defined)) {
                return false;
            }
        }

        // 移除参数中使用的用例

        auto &du_inst = _inst_list[&*inst];

        for (auto i : du_inst.args) {
            erase_usage(i);
        }

        return true;

    }

    void ProcedureDU::set_inst(BasicBlock::iterator inst, const instruction &nInst) {

        if (static_cast<instruct_cate>(inst->index()) == instruct_cate::rv_inst) {
            auto &&rvi = std::get<RiscVInstruction>(nInst);
            if (rvi.register_arg) {
                return;
            }
        }

        auto &raw_du_inst = instDUInfo(inst);

        std::optional<operant> defined_symbol{std::nullopt};
        defined_symbol = std::visit(get_assigned, *inst);
        *inst = nInst;

        if (defined_symbol) {

            std::optional<operant> new_symbol = std::visit(get_assigned, nInst);

            if (new_symbol) {

                rename_define(*defined_symbol, *new_symbol);

            } else {
                LOG_ERROR("replace a instruction that has a definition with a instruction without definition");
            }
        }

        // 处理参数的用例
        // 删除先前参数的用例
        for (auto i : raw_du_inst.args) {
            erase_usage(i);
        }
        // 添加新的用例
        auto arg_len = arg_size(nInst);
        std::vector<usage_pointer> arg_arr;
        for (std::size_t i = 0; i<arg_len; ++i) {
            arg_arr.push_back(add_usage(raw_du_inst.block, raw_du_inst.inst, arg_at(nInst, i), i));
        }
        raw_du_inst.args = std::move(arg_arr);
    }

    DUInst & ProcedureDU::instDUInfo(BasicBlock::iterator inst) {
        if (static_cast<instruct_cate>(inst->index()) == instruct_cate::rv_inst) {
            auto &&rvi = std::get<RiscVInstruction>(*inst);
            if (rvi.register_arg) {
                throw std::logic_error("Acquire a RiscV instruction's def-usage information");
            }
        }
        return _inst_list.at(&*inst);
    }

    DefineEntry ProcedureDU::defined_entry(value_type var) const {
        auto local_storage = local_defined(var);
        if (local_storage) {
            return *local_storage;
        } else {
            if (auto im_s = std::get_if<im_symbol>(&var.symbol); im_s) {
                // 立即数
                return DefineEntry{var.im()};
            } else if (var.var() == procedure.context()->depressed_symbol) {
                // 弃用符号
                return DefineEntry(var.var(), nullptr, SymbolDefineType::meaningless);
            } else {
                // 全局符号
                return DefineEntry(var.var(), nullptr, SymbolDefineType::global);
            }
        }
    }

    const DefineEntry *ProcedureDU::local_defined(const value_type& var) const {
        if (_define_list.contains(var)) {
            auto &define_entry = _define_list.at(var);
            return &define_entry;
        } else if (var.symbolType == symbol_type::im){
            _define_list.insert({std::piecewise_construct, std::tuple{var}, std::tuple{var.im()}});
            return &_define_list.at(var);
        } else {
            return nullptr;
        }
    }

    DefineEntry *ProcedureDU::local_defined(const value_type& var) {
        return const_cast<DefineEntry*>(const_cast<const ProcedureDU*>(this)->local_defined(var));
    }

    ranges::subrange<std::list<UsageEntry>::iterator>
    ProcedureDU::usage_in_block(const value_type& var, BasicBlock *block) {
        auto &li = _usage_list[var][block];
        return {li.begin(), li.end()};
    }

    usage_ref_list_type ProcedureDU::usage_set(const value_type& var) {
        usage_ref_list_type rt;
        for (auto &[_, li] : _usage_list[var]) {
            for (auto &i : li) {
                rt.emplace_back(i);
            }
        }
        return rt;
    }

    void ProcedureDU::add_func_param(const var_symbol& var) {
        DefineEntry de(var);
        _define_list.insert({var, de});
    }

    void ProcedureDU::add_block_param(BasicBlock *block, const var_symbol& var) {
        DefineEntry de(var, block);
        _define_list.insert({var, de});
    }

    bool ProcedureDU::remove_define(const value_type& val) {
        auto &usages = _usage_list[val];
        using ele_type = std::remove_cvref_t<decltype(usages)>;
        using toCheck = ele_type::mapped_type;
        if (ranges::all_of(usages, &toCheck::empty, &ele_type::value_type::second)) {
            auto &defE = *local_defined(val);
            if (defE.du_inst()) {
                _inst_list.erase(&*defE.du_inst()->inst);
            }
            _define_list.erase(val);
            _usage_list.erase(val);
        } else {
            LOG_WARN("try to remove a definition that have usage");
            return false;
        }
        return true;
    }

    std::pair<usage_list_type &, usage_list_type::iterator>
    ProcedureDU::usage_insert_pos(const value_type& var, BasicBlock *block, BasicBlock::iterator inst) {
        auto &to_insert_list = _usage_list[var][block];
        ++inst;
        if (to_insert_list.empty()) {
            return {to_insert_list, to_insert_list.begin()};
        } else {
            auto to_insert_pos = --ranges::find(to_insert_list, inst, &UsageEntry::inst);
            return {to_insert_list, to_insert_pos};
        }
    }

    usage_pointer
    ProcedureDU::add_usage(BasicBlock *block, BasicBlock::iterator inst, value_type var, std::size_t ind) {
        if (static_cast<instruct_cate>(inst->index()) == instruct_cate::rv_inst) {
            auto &&rvi = std::get<RiscVInstruction>(*inst);
            if (rvi.register_arg) {
                throw std::logic_error("Try to trace RiscV Instruction");
            }
        }
        UsageEntry ue{local_defined(var), block, inst, ind};
        auto [list, pos] = usage_insert_pos(var, block, inst);
        return list.insert(pos, ue);
    }

    void ProcedureDU::erase_usage(usage_pointer ptr) {
        auto var = ptr->opr();
        auto block = ptr->block;
        _usage_list[var][block].erase(ptr);
    }

    void ProcedureDU::rename_define(const value_type &val, const value_type &nVal) {
        if (val == nVal) {
            return;
        }
        auto define_entry = local_defined(val);
        auto new_define_entry = local_defined(nVal);
        if (!new_define_entry) {
            define_entry->defined = nVal;
             _define_list[nVal] = std::move(*define_entry);
             define_entry = &_define_list[nVal];
             _define_list.erase(val);
        } else {
            define_entry = new_define_entry;
            _define_list.erase(val);
        }

        // 处理定义名
        auto define_inst = define_entry->inst();
        if (define_inst) {
            std::visit([nVal](auto && PH1) { set_assigned(std::forward<decltype(PH1)>(PH1), nVal.var()); }, **define_inst);
        }

        // 处理用例名

        auto usages = usage_set(val);
        for (UsageEntry& i : usages) {
            i.setOpr(nVal);
            i.defined = define_entry;
        }
        // 转移用例索引
        auto &nMap = _usage_list[nVal];
        for (auto &[k, v] : _usage_list[val]) {
            auto &toInsert = nMap[k];
            toInsert.splice(toInsert.end(), v);
        }

    }
} // IR