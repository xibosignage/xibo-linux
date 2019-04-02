#include "Logger.hpp"

#include <spdlog/spdlog.h>

std::shared_ptr<Logger> Logger::globalLogger;

std::shared_ptr<Logger> Logger::create(const std::string& name, const std::vector<spdlog::sink_ptr>& sinks)
{
    globalLogger = std::shared_ptr<Logger>(new Logger{name, sinks});
    return globalLogger;
}

std::shared_ptr<Logger> Logger::get()
{
    return globalLogger;
}

void Logger::setLevel(LoggingLevel level)
{
    m_combinedLogger->set_level(toSpdlogLevel(level));
}

void Logger::setPattern(const std::string& pattern)
{
    m_combinedLogger->set_pattern(pattern);
}

void Logger::flush()
{
    m_combinedLogger->flush();
}

Logger::Logger(const std::string& name, const std::vector<spdlog::sink_ptr>& sinks)
{
    m_combinedLogger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
    spdlog::register_logger(m_combinedLogger);
}

spdlog::level::level_enum Logger::toSpdlogLevel(LoggingLevel level)
{
    switch(level)
    {
        case LoggingLevel::Trace:
            return spdlog::level::level_enum::trace;
        case LoggingLevel::Debug:
            return spdlog::level::level_enum::debug;
        case LoggingLevel::Info:
            return spdlog::level::level_enum::info;
        case LoggingLevel::Error:
            return spdlog::level::level_enum::err;
    }

    return spdlog::level::level_enum::off;
}
