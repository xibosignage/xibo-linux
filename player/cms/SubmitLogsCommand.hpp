#pragma once

#include "SubmitXmdsCommand.hpp"
#include "cms/xmds/SubmitLog.hpp"

class SubmitLogsCommand : public SubmitXmdsCommand<SubmitLog>
{
public:
    static std::unique_ptr<SubmitLogsCommand> create(const std::string& host,
                                                     const std::string& serverKey,
                                                     const std::string& hardwareKey);

private:
    SubmitLogsCommand(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    void prepare(SubmitLog::Request& request) override;
};
