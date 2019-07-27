#include "ScheduleSerializer.hpp"

#include "networking/xmds/Resources.hpp"
#include "common/fs/FilePath.hpp"
#include "common/Utils.hpp"
#include "common/DateTimeProvider.hpp"

#include <boost/property_tree/xml_parser.hpp>

namespace Resources = XmdsResources::Schedule;

const char* ScheduleParseException::what() const noexcept
{
    return "Schedule is invalid";
}

LayoutSchedule ScheduleSerializer::parseSchedule(const FilePath& path)
{
    try
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(path.string(), tree);

        return parseScheduleImpl(tree);
    }
    catch (std::exception&)
    {
        throw ScheduleParseException{};
    }
}

LayoutSchedule ScheduleSerializer::parseSchedule(const std::string& xmlSchedule)
{
    try
    {
        return parseScheduleImpl(Utils::parseXmlFromString(xmlSchedule));
    }
    catch (std::exception&)
    {
        throw ScheduleParseException{};
    }
}

LayoutSchedule ScheduleSerializer::parseScheduleImpl(const xml_node &scheduleXml)
{    
    LayoutSchedule schedule;
    auto rootNode = scheduleXml.get_child(Resources::Schedule);
    auto attrs = rootNode.get_child(Resources::Attrs);

    schedule.generatedTime = attrs.get<std::string>("generated");

    for(auto [name, node] : rootNode)
    {
        if(name == Resources::Layout)
            schedule.regularLayouts.emplace_back(parseScheduledLayout(node));
        else if(name == Resources::DefaultLayout)
            schedule.defaultLayout = parseDefaultLayout(node);
        else if(name == Resources::Overlays)
            schedule.overlayLayouts = parseOverlayLayouts(node);
        else if(name == Resources::GlobalDependants)
            schedule.globalDependants = parseDependants(node);
    }

    return schedule;
}

ScheduledLayout ScheduleSerializer::parseScheduledLayout(const xml_node& layoutNode)
{
    namespace LayoutAttrs = Resources::LayoutAttrs;

    auto attrs = layoutNode.get_child(Resources::Attrs);
    ScheduledLayout layout;

    layout.scheduleId = attrs.get<int>(LayoutAttrs::ScheduleId);
    layout.id = attrs.get<int>(LayoutAttrs::Id);
    layout.startDT = DateTimeProvider::fromString(attrs.get<std::string>(LayoutAttrs::StartDT));
    layout.endDT = DateTimeProvider::fromString(attrs.get<std::string>(LayoutAttrs::EndDT));
    layout.priority = attrs.get<int>(LayoutAttrs::Priority);

    if(auto dependants = layoutNode.get_child_optional(LayoutAttrs::Dependants))
    {
        layout.dependants = parseDependants(dependants.value());
    }

    return layout;
}

DefaultScheduledLayout ScheduleSerializer::parseDefaultLayout(const xml_node& layoutNode)
{
    namespace LayoutAttrs = Resources::LayoutAttrs;

    auto attrs = layoutNode.get_child(Resources::Attrs);
    DefaultScheduledLayout layout;

    layout.id = attrs.get<int>(LayoutAttrs::Id);

    if(auto dependants = layoutNode.get_child_optional(LayoutAttrs::Dependants))
    {
        layout.dependants = parseDependants(dependants.value());
    }

    return layout;
}

std::vector<ScheduledLayout> ScheduleSerializer::parseOverlayLayouts(const xml_node& overlaysNode)
{
    std::vector<ScheduledLayout> overlayLayouts;

    for(auto [name, node] : overlaysNode)
    {
        overlayLayouts.emplace_back(parseScheduledLayout(node));
    }

    return overlayLayouts;
}

std::vector<std::string> ScheduleSerializer::parseDependants(const xml_node& dependantsNode)
{
    std::vector<std::string> dependants;

    for(auto&& [name, file] : dependantsNode)
    {
        dependants.emplace_back(file.get_value<std::string>());
    }

    return dependants;
}
