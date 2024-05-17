/// @file 日志库的实现文件

#include "Logger.h"

namespace SysYust {
    void Logger::setLogger(Logger *e) noexcept {
        _global.reset(e);
    }

    Logger &Logger::getLogger() noexcept {
        return *_global;
    }

    void Logger::cleanLogger() noexcept {
        _global.reset();
    }

    void Logger::log(const Record &record) {
        if (filter(record)) {
            return ;
        }
        write(fmt::format(pattern,
                             fmt::arg("level", LoggerLevelIdStr[static_cast<std::size_t>(record.level)]),
                             fmt::arg("file", record.file),
                             fmt::arg("line", record.line),
                             fmt::arg("func", record.func),
                             fmt::arg("message", record.message)
                             ));
    }

    bool Logger::filter(const Logger::Record &record) {
        return record.level > currentLevel;
    }

    std::unique_ptr<Logger> Logger::_global{};

} // SysYust