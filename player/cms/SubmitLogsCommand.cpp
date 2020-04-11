#include "SubmitLogsCommand.hpp"

#include "common/logger/XmlLogsRetriever.hpp"

SubmitLogsCommand::SubmitLogsCommand(const std::string& host,
                                     const std::string& serverKey,
                                     const std::string& hardwareKey) :
    SubmitXmdsCommand(host, serverKey, hardwareKey)
{
}

std::unique_ptr<SubmitLogsCommand> SubmitLogsCommand::create(const std::string& host,
                                                             const std::string& serverKey,
                                                             const std::string& hardwareKey)
{
    return std::unique_ptr<SubmitLogsCommand>(new SubmitLogsCommand(host, serverKey, hardwareKey));
}

void SubmitLogsCommand::prepare(SubmitLog::Request& request)
{
    XmlLogsRetriever logsRetriever;
    request.logXml = std::string("<![CDATA[") + logsRetriever.retrieveLogs() + "]]>";
}
