#pragma once

#include <string>

class XmlLogsRetriever
{
public:
    std::string retrieveLogs();

private:
    std::string formatLogs(const std::string& logs);
};
