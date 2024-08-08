/// @file
/// 调试日志库头文件

#ifndef SYSYUST_UTILITY_LOGGER_H
#define SYSYUST_UTILITY_LOGGER_H

#include <string>
#include <memory>
#include <cstdarg>

#include <fmt/core.h>

#include "Trace.h"


namespace SysYust {

    /**
     * @brief 日志等级
     */
    enum class LoggerLevel {
        no = 0,
        fatal, ///< 造成必须立刻终结程序的错误
        error, ///< 接受或造成错误的结果
        warn, ///< 接受或造成可能的异常结果
        info, ///< 程序正常的提示日志
        trace, ///< 更详细的追踪信息
        debug, ///< 额外的调试信息
        all,
    };

    /**
     * @brief 日志等级的字符串描述
     */
    const std::string LoggerLevelIdStr[] = {
            "No",
            "Fatal",
            "Error",
            "Warn",
            "Info",
            "Trace",
            "Debug",
            "All",
    };

    /**
     * @brief 日志器基类
     * @details 日志器可以接受信息，制作日志条目，并记录。该基类提供接受信息的接口和日志条目的格式化支持，记录由子类实现。
     * 日志条目包含以下信息：
     *
     * - 日志点的文件名 file
     * - 日志点的函数名 func
     * - 日志点的行号 line
     * - 一行传入的日志信息 message
     * - 日志的等级 level
     *
     * 程序可以构建多个日志器，并将其中之一设置为全局日志器
     */
    class Logger {
    public:
        Logger() = default;
        Logger(const Logger&) = delete;
        virtual ~Logger() = default;

        /**
         * @brief 包含一个筛选和格式化一个日志记录所需要的信息
         */
        struct Record {
            std::string_view file; ///< 文件名
            const Trace& func; ///< 函数名, 使用 Trace 进行追踪
            std::string message; ///< 用户提供的日志消息
            std::size_t line; ///< 进行记录的代码行数
            LoggerLevel level; ///< 日志等级
        };

        /**
         * @brief 设置当前的全局日志器
         * @param e 将要设置为全局日志器的日志器的指针，设置后 Logger 类接管该对象。
         */
        static void setLogger(Logger* e) noexcept;
        /**
         * @brief 获取当前的全局日志器
         * @return 全局日志器的指针，如果未设置则返回 nullptr。
         */
        static Logger& getLogger() noexcept;
        /**
         * @brief 析构并移除移除全局日志器
         */
        static void cleanLogger() noexcept;

        /**
         * @brief 对日志进行格式化并记录的原语
         * @param record 需要处理的日志记录
         */
         void log(const Record&);

         /**
          * @brief 设置日志格式化的格式
          * @details 格式化字符串使用 `fmt` 库的格式. 所有参数以具名参数提供
          *
          * - file 当前文件名
          * - line 当前行号
          * - func 当前函数名
          * - level 记录等级
          *
          * @param pattern 格式字符串. 使用 `fmt` 库的格式.
          */
        void setPattern(std::string_view pattern);

        /**
         * @brief 获取格式化字符串
         * @return 当前的格式化字符串
         */
        std::string getPattern();

        /**
         * @brief 设置消息被记录的最低等级
         * @param level 过滤等级
         */
        void setLevel(LoggerLevel level);

        /**
         * @brief 获取当前消息等级过滤的等级
         * @return 过滤的等级
         */
        LoggerLevel getLevel();

    protected:

        /**
         * @brief 过滤器选择哪些日志应该被过滤而不被记录
         * @return 如果不记录该记录则返回true.
         */
        virtual bool filter(const Record&);


        /**
         * @brief 所有格式化的日志都会最总调用该函数进行记录，由派生类实现
         * @param record 经过格式化的日志
         */
        virtual void write(std::string_view record) = 0;

    private:
        LoggerLevel _currentLevel = LoggerLevel::all; ///< 记录的日志等级
        std::string _pattern = "[{level} {file}:{line} @ {func}]:{message}\n";

        static std::unique_ptr<Logger> _global;
    };

    /**
     * @brief 内部实现用类,用于进行站追踪日志
     * @details 通过一个包含栈踪信息的 Record 对象进行构造, 使用 RAII 控制日志消息发出的时机.
     */
    class _tracer {
    public:
        explicit _tracer(Logger::Record  r);
        ~_tracer();
    private:
        Logger::Record _r;
    };

#ifdef NDEBUG
#define NLOG
#endif

#define LOG_RECORD(level, message) (::SysYust::Logger::Record{__FILE__, TRACER, ((message)), __LINE__, ((level))})
#ifndef NLOG
#define LOG(level, message) (::SysYust::Logger::getLogger().log(LOG_RECORD(((level)), ((message)))))
#else
#define LOG(level, message) (0)
#endif
#define LOG_FMT(level, ...) LOG(((level)), ::fmt::format(__VA_ARGS__))
#define LOG_INFO(...) LOG_FMT(::SysYust::LoggerLevel::info,  __VA_ARGS__)
#define LOG_ERROR(...) LOG_FMT(::SysYust::LoggerLevel::error,  __VA_ARGS__)
#define LOG_FATAL(...) LOG_FMT(::SysYust::LoggerLevel::fatal,  __VA_ARGS__)
#define LOG_WARN(...) LOG_FMT(::SysYust::LoggerLevel::warn,  __VA_ARGS__)
#define LOG_TRACE(...) LOG_FMT(::SysYust::LoggerLevel::trace,  __VA_ARGS__)
#define LOG_DEBUG(...) LOG_FMT(::SysYust::LoggerLevel::debug,  __VA_ARGS__)
#define LOG_STACK(...) TRACEPOINT(__VA_ARGS__); \
::SysYust::_tracer _stack_printer(LOG_RECORD(::SysYust::LoggerLevel::trace, ""))
} // SysYust

#endif //SYSYUST_UTILITY_LOGGER_H
