#include "Logger.hpp"

#include <spdlog/spdlog.h>

static std::shared_ptr<XiboLogger> g_xiboLogger;

std::shared_ptr<XiboLogger> XiboLogger::create(const std::string& name, const std::vector<spdlog::sink_ptr>& sinks)
{
    g_xiboLogger = std::shared_ptr<XiboLogger>(new XiboLogger{name, sinks});
    return g_xiboLogger;
}

std::shared_ptr<XiboLogger> XiboLogger::get()
{
    return g_xiboLogger;
}

void XiboLogger::setLevel(LoggingLevel level)
{
    combinedLogger_->set_level(toSpdlogLevel(level));
}

void XiboLogger::setPattern(const std::string& pattern)
{
    combinedLogger_->set_pattern(pattern);
}

void XiboLogger::flush()
{
    combinedLogger_->flush();
}

XiboLogger::XiboLogger(const std::string& name, const std::vector<spdlog::sink_ptr>& sinks)
{
    combinedLogger_ = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
    spdlog::register_logger(combinedLogger_);
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
