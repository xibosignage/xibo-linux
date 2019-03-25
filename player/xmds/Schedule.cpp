#include "Schedule.hpp"
#include "Resources.hpp"

namespace Resources = XmdsResources::Schedule;

Soap::RequestSerializer<Schedule::Request>::RequestSerializer(const Schedule::Request& request) : BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<Schedule::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey);
}

Soap::ResponseParser<Schedule::Result>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

Schedule::Result Soap::ResponseParser<Schedule::Result>::doParse(const xml_node& scheduleNode)
{
    auto scheduleXml = scheduleNode.get<std::string>(Resources::ScheduleXml);
    auto schedule = Utils::parseXmlFromString(scheduleXml).get_child(Resources::Schedule);
    Schedule::Result result;

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

ScheduledLayout Soap::ResponseParser<Schedule::Result>::parseScheduledLayout(const xml_node& layoutNode)
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

DefaultScheduledLayout Soap::ResponseParser<Schedule::Result>::parseDefaultLayout(const xml_node& layoutNode)
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

std::vector<std::string> Soap::ResponseParser<Schedule::Result>::parseDependants(const xml_node& dependantsNode)
{
    std::vector<std::string> dependants;

    for(auto [name, file] : dependantsNode)
    {
        dependants.emplace_back(file.get_value<std::string>());
    }

    return dependants;
}
