#pragma once

#include "XmdsCommand.hpp"
#include "cms/xmds/Schedule.hpp"
#include "schedule/LayoutSchedule.hpp"

class GetScheduleCommand : public XmdsCommand<Schedule>
{
    using SignalScheduleReady = boost::signals2::signal<void(const LayoutSchedule&)>;

public:
    static std::unique_ptr<GetScheduleCommand> create(const std::string& host,
                                                      const std::string& serverKey,
                                                      const std::string& hardwareKey);

    SignalScheduleReady& scheduleReady();

protected:
    Schedule::Request prepareRequest() override;
    void processResponse(const Schedule::Response& response) override;

private:
    GetScheduleCommand(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

private:
    std::string serverKey_;
    std::string hardwareKey_;
    SignalScheduleReady scheduleReady_;
};
