//
// Created by LL06p on 24-7-27.
//

#ifndef SYSYUST_INITINFO_H
#define SYSYUST_INITINFO_H

#include "IR/SymbolUtil.h"
#include "IR/TypeUtil.h"

namespace SysYust::IR {

    class InitInfo {
    public:
        using value_type = im_symbol;
        using var_type = var_symbol;
        using list_type = std::vector<InitInfo*>;
        using data_type = std::variant<value_type, var_type, list_type>;

        InitInfo() = default;
        /*NOLINT*/ InitInfo(var_symbol var);
        /*NOLINT*/ InitInfo(im_symbol im);
        /*NOLINT*/ InitInfo(const list_type &li);
        /*NOLINT*/ InitInfo(list_type &&li);
        /*NOLINT*/ InitInfo(const std::vector<im_symbol>& li);
        InitInfo(const InitInfo &);
        InitInfo(InitInfo &&) noexcept;
        InitInfo& operator= (const InitInfo &);
        InitInfo& operator= (InitInfo &&) noexcept ;

        [[nodiscard]] bool is_list() const {
            return _data.index() == 1;
        }
        [[nodiscard]] value_type value() const {
            return std::get<value_type>(_data);
        }
        [[nodiscard]] const std::vector<InitInfo*>& list() const {
            return std::get<list_type>(_data);
        }
        [[nodiscard]] var_type var() const {
            return std::get<var_type>(_data);
        }


    private:
        void release();
        static list_type copy_list(const list_type &li);
        data_type _data = im_symbol(im_symbol::undef);
    };

} // IR

#endif //SYSYUST_INITINFO_H
