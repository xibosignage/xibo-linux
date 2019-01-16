#include "Schedule.hpp"
#include "Resources.hpp"

namespace Resources = XMDSResources::Schedule;

SOAP::RequestSerializer<Schedule::Request>::RequestSerializer(const Schedule::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<Schedule::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey);
}

SOAP::ResponseParser<Schedule::Result>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

Schedule::Result SOAP::ResponseParser<Schedule::Result>::doParse(const boost::property_tree::ptree& scheduleNode)
{
    auto scheduleXml = scheduleNode.get<std::string>(Resources::ScheduleXml);
    auto schedule = Utils::parseXmlFromString(scheduleXml).get_child(Resources::Schedule);
    Schedule::Result result;

//    Log::debug(scheduleXml);

    for(auto [name, node] : schedule)
    {
        if(name == Resources::Layout)
            result.layouts.emplace_back(parseScheduledLayout(node));
        else if(name == Resources::DefaultLayout)
            result.defaultLayout = parseDefaultLayout(node);
        else if(name == Resources::GlobalDependants)
            result.globalDependants = parseDependants(node);
    }

    return result;
}

ScheduledLayout SOAP::ResponseParser<Schedule::Result>::parseScheduledLayout(const boost::property_tree::ptree& layoutNode)
{
    namespace LayoutAttrs = Resources::LayoutAttrs;

    auto attrs = layoutNode.get_child(LayoutAttrs::Node);
    ScheduledLayout layout;

    layout.scheduleId = attrs.get<int>(LayoutAttrs::ScheduleId);
    layout.id = attrs.get<int>(LayoutAttrs::Id);
    layout.startDT = boost::posix_time::time_from_string(attrs.get<std::string>(LayoutAttrs::StartDT));
    layout.endDT = boost::posix_time::time_from_string(attrs.get<std::string>(LayoutAttrs::EndDT));
    layout.priority = attrs.get<int>(LayoutAttrs::Priority);

    if(auto dependants = layoutNode.get_child_optional(LayoutAttrs::Dependants))
    {
        layout.dependants = parseDependants(dependants.value());
    }

    return layout;
}

DefaultScheduledLayout SOAP::ResponseParser<Schedule::Result>::parseDefaultLayout(const boost::property_tree::ptree& layoutNode)
{
    namespace LayoutAttrs = Resources::LayoutAttrs;

    auto attrs = layoutNode.get_child(LayoutAttrs::Node);
    DefaultScheduledLayout layout;

    layout.id = attrs.get<int>(LayoutAttrs::Id);

    if(auto dependants = layoutNode.get_child_optional(LayoutAttrs::Dependants))
    {
        layout.dependants = parseDependants(dependants.value());
    }

    return layout;
}

std::vector<std::string> SOAP::ResponseParser<Schedule::Result>::parseDependants(const boost::property_tree::ptree& dependantsNode)
{
    std::vector<std::string> dependants;

    for(auto [name, file] : dependantsNode)
    {
        dependants.emplace_back(file.get_value<std::string>());
    }

    return dependants;
}
