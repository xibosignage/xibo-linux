#include "XmlLogsRetriever.hpp"

#include "common/parsing/Parsing.hpp"
#include "common/logger/Logging.hpp"
#include "common/logger/XmlLogsRepo.hpp"

// TODO: move to SubmitLogs request where it should format logs according to required CMS format

std::string XmlLogsRetriever::retrieveLogs()
{
    auto&& xmlLogsRepo = XmlLogsRepo::get();
    Log::logger()->flush();

    auto logs = formatLogs(xmlLogsRepo.buffer());
    xmlLogsRepo.clear();
    return logs;
}

std::string XmlLogsRetriever::formatLogs(const std::string& logs)
{
    XmlNode formattedLogsNode;

    try
    {
        auto logsNode = Parsing::xmlFrom(logs);
        formattedLogsNode.add_child("logs", logsNode);

        return Parsing::xmlTreeToString(formattedLogsNode);
    }
    catch (std::exception& e)
    {
        Log::error("[XmlLogsRetriever] Format logs error: {}", e.what());
        Log::trace("[XmlLogsRetriever] {}", logs);
    }

    return {};
}
