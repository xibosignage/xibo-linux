#include "SubmitStatsCommand.hpp"

#include "stat/StatsFormatter.hpp"
#include "stat/StatsRecorder.hpp"

SubmitStatsCommand::SubmitStatsCommand(const std::string& host,
                                       const std::string& serverKey,
                                       const std::string& hardwareKey,
                                       StatsRecorder& statsRecorder) :
    SubmitXmdsCommand(host, serverKey, hardwareKey),
    statsRecorder_(statsRecorder)
{
}

std::unique_ptr<SubmitStatsCommand> SubmitStatsCommand::create(const std::string& host,
                                                               const std::string& serverKey,
                                                               const std::string& hardwareKey,
                                                               StatsRecorder& statsRecorder)
{
    return std::unique_ptr<SubmitStatsCommand>(new SubmitStatsCommand(host, serverKey, hardwareKey, statsRecorder));
}

void SubmitStatsCommand::prepare(SubmitStats::Request& request)
{
    StatsFormatter formatter;
    request.statXml = std::string("<![CDATA[") + formatter.toXml(statsRecorder_.records()) + "]]>";
}
