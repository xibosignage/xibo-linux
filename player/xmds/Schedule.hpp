#pragma once

#include "Field.hpp"
#include "SOAP.hpp"
#include "BaseRequestSerializer.hpp"
#include "BaseResponseParser.hpp"

#include "managers/ScheduleItem.hpp"

namespace Schedule
{
    struct Result
    {
        std::vector<std::string> globalDependants;
        std::vector<ScheduledLayout> layouts;
        DefaultScheduledLayout defaultLayout;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
    };
}

template<>
class SOAP::RequestSerializer<Schedule::Request> : public BaseRequestSerializer<Schedule::Request>
{
public:
    RequestSerializer(const Schedule::Request& request);
    std::string string();

};

template<>
class SOAP::ResponseParser<Schedule::Result> : public BaseResponseParser<Schedule::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    Schedule::Result doParse(const boost::property_tree::ptree& scheduleNode) override;

private:
    ScheduledLayout parseScheduledLayout(const boost::property_tree::ptree& layoutNode);
    DefaultScheduledLayout parseDefaultLayout(const boost::property_tree::ptree& layoutNode);
    std::vector<std::string> parseDependants(const boost::property_tree::ptree& dependantsNode);

};
