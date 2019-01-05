#pragma once

#include "Field.hpp"
#include "SOAP.hpp"
#include "BaseRequestSerializer.hpp"
#include "BaseResponseParser.hpp"

namespace Schedule
{
    struct Response
    {
        struct DefaultLayout
        {
            int id;
            std::vector<std::string> dependants;
        };

        struct Layout
        {
            int scheduleId;
            int id;
            int priority;
            std::string startDT;
            std::string endDT;
            std::vector<std::string> dependants;
        };

        std::vector<std::string> globalDependants;
        std::vector<Layout> layouts;
        DefaultLayout defaultLayout;
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
class SOAP::ResponseParser<Schedule::Response> : public BaseResponseParser<Schedule::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    Schedule::Response doParse(const boost::property_tree::ptree& scheduleNode) override;

private:
    Schedule::Response::Layout parseScheduledLayout(const boost::property_tree::ptree& layoutNode);
    Schedule::Response::DefaultLayout parseDefaultLayout(const boost::property_tree::ptree& layoutNode);
    std::vector<std::string> parseDependants(const boost::property_tree::ptree& dependantsNode);

};
