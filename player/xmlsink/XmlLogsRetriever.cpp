#include "XmlLogsRetriever.hpp"

#include "XmlLogsRepo.hpp"

#include "common/logger/Logger.hpp"
#include "common/Utils.hpp"

#include <boost/property_tree/ptree.hpp>

std::string XmlLogsRetriever::retrieveLogs()
{
    auto&& xmlLogsRepo = XmlLogsRepo::get();
    Logger::get()->flush();

    auto logs = formatLogs(xmlLogsRepo.buffer());
    xmlLogsRepo.clear();
    return logs;
}

std::string XmlLogsRetriever::formatLogs(const std::string& logs)
{
    xml_node formattedLogsNode;
    auto logsNode =  Utils::parseXmlFromString(logs);
    formattedLogsNode.add_child("logs", logsNode);
    return Utils::xmlTreeToEscapedString(formattedLogsNode);
}
