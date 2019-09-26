#include "XmlLogsRetriever.hpp"

#include "XmlLogsRepo.hpp"

#include "common/Parsing.hpp"
#include "common/logger/Logging.hpp"

#include <boost/property_tree/ptree.hpp>

std::string XmlLogsRetriever::retrieveLogs()
{
    auto&& xmlLogsRepo = XmlLogsRepo::get();
    XiboLogger::get()->flush();

    auto logs = formatLogs(xmlLogsRepo.buffer());
    xmlLogsRepo.clear();
    return logs;
}

std::string XmlLogsRetriever::formatLogs(const std::string& logs)
{
    ptree_node formattedLogsNode;

    try
    {
        auto logsNode = Parsing::xmlFromString(logs);
        formattedLogsNode.add_child("logs", logsNode);

        return Parsing::xmlTreeToString(formattedLogsNode);
    }
    catch (std::exception& e)
    {
        Log::error("XML logs error: ", e.what());
        Log::trace(logs);
    }

    return {};
}
