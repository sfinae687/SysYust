//
// Created by LL06p on 24-7-4.
//

#ifndef SYSYUST_AST_SYMBOLTABLE_H
#define SYSYUST_AST_SYMBOLTABLE_H

#include <unordered_map>
#include <memory>
#include <cassert>

#include "utility/IdAllocator.h"

namespace SysYust::AST {

    /**
     * @brief 符号表，通过 NumId 定位一个符号记录
     * @tparam E 符号记录的类型
     */
    template<typename E>
    class SymbolTable {
    public:
        using entry_t = E;
        using name_t = std::string;

        SymbolTable() = default;
        explicit SymbolTable(SymbolTable* parent)
        : _parent(parent) {

        }

        /**
         * @brief 获取当前符号表的父符号表
         */
        SymbolTable* getParent() {
            return _parent;
        }

        /**
         * @brief 检测符号表链中是否存在特定 id 的符号
         */
        [[nodiscard]] bool contains(NumId id) const {
            return _local_entry.contains(id) || _parent && _parent->contains(id);
        }

        /**
         * @brief 获取对应 id 的符号条目，如果之后设置了符号表中的任意条目那么引用可能空悬
         */
        const entry_t& getInfo(NumId id) const {
            return seek(id);
        }

        /**
         * @brief 设置特定 id 的符号条目
         */
        void setInfo(NumId id, const entry_t &e) {
            seek(id) = e;
        }

        /**
         * @brief 设置特定 id 的条目，使用移动语义
         */
        void setInfo(NumId id, entry_t &&e) {
            seek(id) = std::move(e);
        }

    private:

        const entry_t& seek(NumId id) const {
            if (_local_entry.contains(id)) {
                return _local_entry.at(id);
            } else if (_parent) {
                return _parent->seek(id);
            } else {
                assert(false); // 不检查带查找条目是否存在
            }
        }

        entry_t& seek(NumId id) {
            auto rt = const_cast<const SymbolTable*>(this)->seek(id);
            return const_cast<entry_t&>(rt);
        }

        SymbolTable *_parent = nullptr;
        std::unordered_map<NumId, entry_t> _local_entry = {};
    };

} // AST

#endif //SYSYUST_AST_SYMBOLTABLE_H
