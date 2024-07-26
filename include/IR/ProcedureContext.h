//
// Created by LL06p on 24-7-24.
//

#ifndef SYSYUST_PROCEDURECONTEXT_H
#define SYSYUST_PROCEDURECONTEXT_H

#include <string>
#include <unordered_map>

#include "IR/CodeContext.h"
#include "IR/SymbolUtil.h"

namespace SysYust::IR {

    /**
     * @brief 过程级别的上下文，包含了对全局上下文的引用，参数符号信息，生成局部符号的信息。
     */
    class ProcedureContext {
        using revision_map = std::unordered_map<std::string, std::size_t>;
        using param_var_set_t = std::unordered_set<var_symbol>;
    public:
        explicit ProcedureContext(CodeContext &context);

        // 用于检测符号信息的方法

        [[nodiscard]] bool is_param(var_symbol v) const;

        // 用于生成变量符号的一组方法

        std::size_t lastRevision(const var_symbol& v) const;
        std::size_t lastRevision(const std::string& sym) const;
        std::size_t nextRevision(const var_symbol& v);
        std::size_t nextRevision(const std::string& sym);
        var_symbol nextSymbol(const var_symbol& v);
        var_symbol nextSymbol(const std::string& sym);
        var_symbol nextSymbol();

        /**
         * @brief 默认变量符号，无参数的 nextSymbol。
         */
        std::string defaultSymbol = "t";
        var_symbol depressed_symbol{"_", 0, Type::get(Type::none)};
        CodeContext &global;
    private:
        revision_map _nxt_revision{};
        param_var_set_t param_var_set;
    };

} // IR

#endif //SYSYUST_PROCEDURECONTEXT_H
