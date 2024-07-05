/// @file 保存上下文信息的类

#ifndef SYSYUST_ENV_H
#define SYSYUST_ENV_H

#include <string>
#include <memory>

#include "utility/IdAllocator.h"
#include "AST/Env/SymbolTable.h"
#include "AST/Env/VarInfo.h"
#include "AST/Env/FuncInfo.h"

namespace SysYust::AST {

    /**
     * @brief 保存局部上下文信息,只能堆上分配
     */
    class Env {
    public:
        using name_t = std::string;

        Env() = default;
        explicit Env(std::shared_ptr<Env> parent);

        std::shared_ptr<Env> getParent() const ;

        NumId getId(const name_t& name);

        SymbolTable<VarInfo> var_table = {};
        SymbolTable<FuncInfo> func_table = {};
    private:
        std::shared_ptr<IdAllocator<name_t>> _name_id = std::make_shared<IdAllocator<name_t>>();
        std::shared_ptr<Env> _parent = nullptr;
    };

} // AST

#endif //SYSYUST_ENV_H
