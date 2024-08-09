/// @file
/// 日志调试库 Trace 栈的部分追踪

#ifndef SYSYUST_TRACE_H
#define SYSYUST_TRACE_H

#include <utility>
#include <string>
#include <string_view>
#include <list>
#include <tuple>
#include <functional>

#include <fmt/core.h>
#include <fmt/ranges.h>

namespace SysYust {

    /**
     * @brief 手动的栈追踪器,一般不会进行完整的栈追踪
     * @details 每个 Trace 对象表示依次函数调用
     */
    class Trace {
    public:
        static thread_local Trace* TopTrace;

        /**
         * @brief 构建一个追踪点
         * @tparam Args 被记录的参数类型
         * @param func 函数名
         * @param args 被记录的参数的值
         */
        template<typename... Args>
        explicit Trace(std::string_view func, Args&&... args)
            : prevTrace(TopTrace)
            , functionName(func)
            , depth(TopTrace && func == TopTrace->functionName ? TopTrace->depth+1 : 1) {
            TopTrace = this;
            if constexpr (sizeof...(Args) != 0) {
                argRecords = fmt::format("{}", std::make_tuple(std::forward<Args>(args)...));
            } else {
                argRecords = "(void)";
            }
        }

        Trace(const Trace&) = delete;
        Trace(Trace&&) = default;
        ~Trace() {
            TopTrace = prevTrace;
        }

        /**
         * @brief 上一个追踪点
         */
        Trace * const prevTrace;

        /**
         * @brief 当前函数名
         */
        std::string_view functionName;


        /**
         * @brief 递归深度
         */
        const std::size_t depth;

        /**
         * @brief 获取参数记录
         * @return 参数记录的视图
         */
        [[nodiscard]] std::string_view getArgRecords() const {
            return argRecords;
        }

    private:
        /**
         * @brief 函数参数记录
         */
        std::string argRecords;
    };

    inline thread_local Trace* Trace::TopTrace = nullptr;

    inline auto format_as(const Trace& t) {
        if (t.depth > 1) {
            return fmt::format("{}#{}", t.functionName, t.depth);
        } else {
            return fmt::format("{}", t.functionName);
        }
    }

#ifndef NLOG
#define TRACEPOINT(...) ::SysYust::Trace _trace_point(__func__, ##__VA_ARGS__)
#define TRACER (::SysYust::Trace::TopTrace && __func__ == ::SysYust::Trace::TopTrace->functionName ? \
*::SysYust::Trace::TopTrace : \
static_cast<const ::SysYust::Trace&>(::SysYust::Trace(__func__)) )
#else
#define TRACEPOINT(...) (0)
#endif

} // SysYust

#endif //SYSYUST_TRACE_H
