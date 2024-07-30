//
// Created by LL06p on 24-7-27.
//

#include "IR/InitInfo.h"

namespace SysYust::IR {
    InitInfo::InitInfo(im_symbol im)
        : _data(im)
    {

    }

    InitInfo::InitInfo(const InitInfo::list_type &li)
        : _data(copy_list(li))
    {

    }

    InitInfo::InitInfo(InitInfo::list_type &&li)
        : _data(std::move(li))
    {

    }

    InitInfo::InitInfo(const std::vector<im_symbol> &li)
        : _data([&] {
           list_type rt;
           for (auto &i : li) {
               rt.push_back(new InitInfo(i));
           }
           return rt;
        }())
    {

    }

    // NOLINTBEGIN(misc-no-recursion)
    InitInfo::InitInfo(const InitInfo &e)
        : _data([&] () -> data_type {
           if (e.is_list()) {
               return {copy_list(e.list())} ;
           } else {
               return e._data;
           }
        }())
    {

    }
    // NOLINTEND(misc-no-recursion)

    InitInfo::InitInfo(InitInfo &&e) noexcept
          : _data(std::move(e._data))
    {

    }

    InitInfo &InitInfo::operator=(const InitInfo &e) {
        release();
        if (e.is_list()) {
            _data = copy_list(e.list());
        } else {
            _data = e._data;
        }
        return *this;
    }

    InitInfo &InitInfo::operator=(InitInfo &&e) noexcept {
        _data = std::move(e._data);
        return *this;
    }

    // NOLINTNEXTLINE(misc-no-recursion)
    InitInfo::list_type InitInfo::copy_list(const InitInfo::list_type &li) {
        list_type rt{};
        for (auto item : li) {
            rt.emplace_back(new InitInfo(*item));
        }
        return rt;
    }

    void InitInfo::release() {
        if (is_list()) {
            auto &li = list();
            if (!li.empty()) {
                for (auto i : li) {
                    delete i;
                }
            }
        }
    }

    InitInfo::InitInfo(var_symbol var)
        : _data(var)
    {

    }
} // IR