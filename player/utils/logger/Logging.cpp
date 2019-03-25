#include "Logging.hpp"

#include "XmlLogsRepo.hpp"

std::shared_ptr<Logger> Log::logger()
{
    static auto logger = Logger::get();
    return logger;
}

std::string Log::xmlLogs()
{
    XmlLogsRetriever logsRetriever{logger(), XmlLogsRepo::get()};
    return logsRetriever.retrieveLogs();
}
