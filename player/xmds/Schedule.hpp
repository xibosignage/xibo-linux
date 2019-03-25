#pragma once

#include "Soap.hpp"
#include "BaseRequestSerializer.hpp"
#include "BaseResponseParser.hpp"

#include "utils/Field.hpp"
#include "managers/LayoutSchedule.hpp"

namespace Schedule
{
    struct Result
    {
        LayoutSchedule schedule;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
    };
}

template<>
class Soap::RequestSerializer<Schedule::Request> : public BaseRequestSerializer<Schedule::Request>
{
public:
    RequestSerializer(const Schedule::Request& request);
    std::string string();

};

template<>
class Soap::ResponseParser<Schedule::Result> : public BaseResponseParser<Schedule::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    Schedule::Result doParse(const xml_node& scheduleNode) override;

private:
    ScheduledLayout parseScheduledLayout(const xml_node& layoutNode);
    DefaultScheduledLayout parseDefaultLayout(const xml_node& layoutNode);
    std::vector<std::string> parseDependants(const xml_node& dependantsNode);

};
