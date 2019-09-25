#pragma once

#include "LoggingLevel.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/logger.h>

class XiboLogger
{
public:
    static std::shared_ptr<XiboLogger> create(const std::string& name, const std::vector<spdlog::sink_ptr>& sinks);
    static std::shared_ptr<XiboLogger> get();

    void setLevel(LoggingLevel level);
    void setPattern(const std::string& pattern);
    void flush();

    template <typename... Args>
    void log(LoggingLevel level, const char* fmt, const Args&... args)
    {
        m_combinedLogger->log(toSpdlogLevel(level), fmt, args...);
    }

    template <typename T>
    void log(LoggingLevel level, const T& arg)
    {
        m_combinedLogger->log(toSpdlogLevel(level), arg);
    }

private:
    XiboLogger(const std::string& name, const std::vector<spdlog::sink_ptr>& sinks);
    spdlog::level::level_enum toSpdlogLevel(LoggingLevel level);

    static std::shared_ptr<XiboLogger> globalLogger;

private:
    std::shared_ptr<spdlog::logger> m_combinedLogger;
};
