/// @file 日志库的实现文件

#include "logger.h"

namespace SysYust {
    void logger::setLogger(logger *e) noexcept {
        _global.reset(e);
    }

    logger &logger::getLogger() noexcept {
        return *_global;
    }

    void logger::cleanLogger() noexcept {
        _global.reset();
    }

    void logger::log(const Record &record) {
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

    bool logger::filter(const logger::Record &record) {
        return record.level > currentLevel;
    }

    std::unique_ptr<logger> logger::_global{};

} // SysYust