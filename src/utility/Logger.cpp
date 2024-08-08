/// @file
/// 日志库的实现文件

#include "Logger.h"

#include <utility>

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
        write(fmt::format(fmt::runtime(_pattern),
                          fmt::arg("level", LoggerLevelIdStr[static_cast<std::size_t>(record.level)]),
                          fmt::arg("file", record.file),
                          fmt::arg("line", record.line),
                          fmt::arg("func", record.func),
                          fmt::arg("message", record.message)
                             ));
    }

    bool Logger::filter(const Logger::Record &record) {
        return record.level > _currentLevel;
    }

    std::unique_ptr<Logger> Logger::_global{};

    LoggerLevel Logger::getLevel() {
        return _currentLevel;
    }

    void Logger::setLevel(LoggerLevel level) {
        _currentLevel = level;
    }

    std::string Logger::getPattern() {
        return _pattern;
    }

    void Logger::setPattern(std::string_view pattern) {
        _pattern = pattern;
    }

    _tracer::_tracer(Logger::Record r)
        : _r(std::move(r)) {
        _r.message = fmt::format("Entry with Args{}", _r.func.getArgRecords());
        Logger::getLogger().log(_r);
    }

    _tracer::~_tracer() {
        _r.message = "Exit";
        Logger::getLogger().log(_r);
    }
} // SysYust