#pragma once

#include <memory>

class Logger;
class XmlLogsRepo;

class XmlLogsRetriever
{
public:
    XmlLogsRetriever(std::shared_ptr<Logger> logger, XmlLogsRepo& xmlLogsRepo);

    std::string retrieveLogs();

private:
    std::string formatLogs(const std::string& logs);

private:
    std::shared_ptr<Logger> m_logger;
    XmlLogsRepo& m_xmlLogsRepo;

};
