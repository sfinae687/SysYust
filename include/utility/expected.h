/// @file expected 模板类的定义

#ifndef SYSYUST_UTILITY_EXPECTED_H
#define SYSYUST_UTILITY_EXPECTED_H

#include <zeus/expected.hpp>

namespace SysYust {

    using zeus::unexpect_t;
    using zeus::unexpect;

    template<typename E>
    using unexpected = zeus::unexpected<E>;

    template<typename T, typename E>
    using expected = zeus::expected<T, E>;

} // SysYust

#endif //SYSYUST_UTILITY_EXPECTED_H
