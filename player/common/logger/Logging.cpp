#include "Logging.hpp"

#include "common/logger/XmlLoggerSink.hpp"

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

const std::string SpdLogger = "logger";

std::shared_ptr<spdlog::logger> Log::logger()
{
    static auto logger = spdlog::get(SpdLogger);
    return logger;
}

std::vector<spdlog::sink_ptr> createLoggerSinks()
{
    std::vector<spdlog::sink_ptr> sinks;

    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    sinks.push_back(std::make_shared<LoggerXmlSinkMt>(XmlLogsRepo::get()));

    return sinks;
}

std::shared_ptr<spdlog::logger> Log::create()
{
    return create(createLoggerSinks());
}

std::shared_ptr<spdlog::logger> Log::create(const std::vector<spdlog::sink_ptr>& sinks)
{
    auto logger = std::make_shared<spdlog::logger>(SpdLogger, sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::debug);
    logger->set_pattern("[%H:%M:%S.%e] [%t] [%l]: %v");
    spdlog::register_logger(logger);
    return logger;
}

void Log::setLevel(const std::string& level)
{
    if (level == "trace")
        logger()->set_level(spdlog::level::trace);
    else if (level == "debug")
        logger()->set_level(spdlog::level::debug);
    else if (level == "info")
        logger()->set_level(spdlog::level::info);
    else if (level == "error")
        logger()->set_level(spdlog::level::err);
}
