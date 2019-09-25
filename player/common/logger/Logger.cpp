#include "Logger.hpp"

#include <spdlog/spdlog.h>

std::shared_ptr<XiboLogger> XiboLogger::globalLogger;

std::shared_ptr<XiboLogger> XiboLogger::create(const std::string& name, const std::vector<spdlog::sink_ptr>& sinks)
{
    globalLogger = std::shared_ptr<XiboLogger>(new XiboLogger{name, sinks});
    return globalLogger;
}

std::shared_ptr<XiboLogger> XiboLogger::get()
{
    return globalLogger;
}

void XiboLogger::setLevel(LoggingLevel level)
{
    m_combinedLogger->set_level(toSpdlogLevel(level));
}

void XiboLogger::setPattern(const std::string& pattern)
{
    m_combinedLogger->set_pattern(pattern);
}

void XiboLogger::flush()
{
    m_combinedLogger->flush();
}

XiboLogger::XiboLogger(const std::string& name, const std::vector<spdlog::sink_ptr>& sinks)
{
    m_combinedLogger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
    spdlog::register_logger(m_combinedLogger);
}

spdlog::level::level_enum XiboLogger::toSpdlogLevel(LoggingLevel level)
{
    switch (level)
    {
        case LoggingLevel::Trace: return spdlog::level::level_enum::trace;
        case LoggingLevel::Debug: return spdlog::level::level_enum::debug;
        case LoggingLevel::Info: return spdlog::level::level_enum::info;
        case LoggingLevel::Error: return spdlog::level::level_enum::err;
    }

    return spdlog::level::level_enum::off;
}
