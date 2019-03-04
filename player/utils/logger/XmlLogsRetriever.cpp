#include "XmlLogsRetriever.hpp"

#include "XmlLogsRepo.hpp"
#include "Logger.hpp"

#include "utils/Utilities.hpp"

#include <boost/property_tree/ptree.hpp>

XmlLogsRetriever::XmlLogsRetriever(std::shared_ptr<Logger> logger) :
    m_logger(logger), m_xmlLogsRepo(logger->xmlLogsRepo())
{
}

std::string XmlLogsRetriever::retrieveLogs()
{
    m_logger->flush();

    auto logs = formatLogs(m_xmlLogsRepo.get());
    m_xmlLogsRepo.clear();
    return logs;
}

std::string XmlLogsRetriever::formatLogs(const std::string& logs)
{
    xml_node formattedLogsNode;
    auto logsNode =  Utils::parseXmlFromString(logs);
    formattedLogsNode.add_child("logs", logsNode);
    return Utils::xmlTreeToEscapedString(formattedLogsNode);
}
