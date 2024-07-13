//
// Created by LL06p on 24-7-3.
//

#ifndef SYSYUST_IDALLOCATOR_H
#define SYSYUST_IDALLOCATOR_H

#include <concepts>
#include <utility>
#include <unordered_map>

namespace SysYust {

    typedef std::size_t NumId;

    /**
     * @brief 分配给 T 一个唯一的 Id。
     * @tparam T 待分配的对象的类型，用户保证它可可以使用 std::hash 进行哈希操作
     */
    template <typename T>
    requires std::equality_comparable<T>
    class IdAllocator {
    public:
        NumId idFor(const T&);
    private:
        NumId _lastUnused = 0;
        std::unordered_map<T, NumId> _idTable{};
    };

    template<typename T>
    requires std::equality_comparable<T>
    NumId IdAllocator<T>::idFor(const T &t) {
        if (!_idTable.contains(t)) {
            return _idTable[t] = _lastUnused++;
        } else {
            return _idTable[t];
        }
    }

} // SysYust

#endif //SYSYUST_IDALLOCATOR_H
