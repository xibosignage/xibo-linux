#pragma once

#include <memory>

class Logger;
class XmlLogsRepo;

class XmlLogsRetriever
{
public:
    std::string retrieveLogs();

private:
    std::string formatLogs(const std::string& logs);

};
