#include "Logger.hpp"

#include "XmlLoggerSink.hpp"

#include <spdlog/sinks/stdout_sinks.h>

std::shared_ptr<Logger> Logger::globalLogger;

std::shared_ptr<Logger> Logger::create(const std::string& name)
{
    globalLogger = std::shared_ptr<Logger>(new Logger{name});
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

XmlLogsRepo& Logger::xmlLogsRepo()
{
    return *m_xmlLogsRepo;
}

Logger::Logger(const std::string& name)
{
    m_xmlLogsRepo = std::make_unique<XmlLogsRepo>();

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    sinks.push_back(std::make_shared<LoggerXmlSinkMt>(xmlLogsRepo()));

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
