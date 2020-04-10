#pragma once

#include "SubmitXmdsCommand.hpp"
#include "cms/xmds/SubmitStats.hpp"

class StatsRecorder;

class SubmitStatsCommand : public SubmitXmdsCommand<SubmitStats>
{
public:
    static std::unique_ptr<SubmitStatsCommand> create(const std::string& host,
                                                      const std::string& serverKey,
                                                      const std::string& hardwareKey,
                                                      StatsRecorder& statsRecorder);

private:
    SubmitStatsCommand(const std::string& host,
                       const std::string& serverKey,
                       const std::string& hardwareKey,
                       StatsRecorder& statsRecorder);

    void prepare(SubmitStats::Request& request) override;

private:
    StatsRecorder& statsRecorder_;
};
