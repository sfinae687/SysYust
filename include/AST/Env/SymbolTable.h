//
// Created by LL06p on 24-7-4.
//

#ifndef SYSYUST_AST_SYMBOLTABLE_H
#define SYSYUST_AST_SYMBOLTABLE_H

#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <memory>
#include <cassert>

#include "utility/IdAllocator.h"
#include "utility/Logger.h"

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
        using sequence_type = std::vector<NumId>;


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
            if (!contains(id)) {
                _seq.push_back(id);
            }
            seek(id) = e;
        }

        /**
         * @brief 设置特定 id 的条目，使用移动语义
         */
        void setInfo(NumId id, entry_t &&e) {
            if (!contains(id)) {
                _seq.push_back(id);
            }
            seek(id) = std::move(e);
        }

        /**
         * @brief 遍历当前作用域
         */
         auto begin() {
             return _local_entry.begin();
         }
         /**
          * @brief 结束迭代器
          */
         auto end() {
             return _local_entry.end();
         }

         const sequence_type& getSequence() const {
            return _seq;
         }
         
    private:
        using table_type = std::unordered_map<NumId, entry_t>;

        const entry_t& seek(NumId id) const {
            LOG_TRACE("Seek {} symbol info with id {}", typeid(E).name(), id);
            if (_local_entry.contains(id)) {
                try {
                    return _local_entry.at(id);
                } catch (std::out_of_range &e) {
                    LOG_ERROR("Exception:{}", e.what());
                    __builtin_unreachable();
                }
            } else if (_parent) {
                return _parent->getInfo(id);
            } else {
                assert(false); // 不检查带查找条目是否存在
            }
        }

        entry_t& seek(NumId id) {
            LOG_TRACE("Seek local {} symbol info with id {}", typeid(E).name(), id);
            return _local_entry[id]; // 写入仅考虑当前作用域
        }

        SymbolTable *_parent = nullptr;
        table_type _local_entry = {};
        sequence_type _seq = {};
    };

} // AST

#endif //SYSYUST_AST_SYMBOLTABLE_H
