#include "ScheduleManager.hpp"

#include "networking/xmds/Resources.hpp"
#include "common/Utils.hpp"
#include "common/FileSystem.hpp"
#include "common/DateTimeProvider.hpp"

#include <boost/property_tree/xml_parser.hpp>

namespace Resources = XmdsResources::Schedule;

void ScheduleManager::load(const FilePath& path)
{
    if(FileSystem::exists(path))
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(path.string(), tree);

        m_schedule = parseSchedule(tree);
    }

    m_path = path;
}

LayoutSchedule ScheduleManager::parseSchedule(const xml_node& scheduleXml)
{
    LayoutSchedule schedule;
    auto rootNode = scheduleXml.get_child(Resources::Schedule);
    auto attrs = rootNode.get_child(Resources::Attrs);

    schedule.updateGeneratedTime(attrs.get<std::string>("generated"));

    for(auto [name, node] : rootNode)
    {
        if(name == Resources::Layout)
            schedule.addScheduledLayout(parseScheduledLayout(node));
        else if(name == Resources::DefaultLayout)
            schedule.updateDefaultLayout(parseDefaultLayout(node));
        else if(name == Resources::Overlays)
            schedule.addOverlayLayouts(parseOverlayLayouts(node));
        else if(name == Resources::GlobalDependants)
            schedule.updateDependants(parseDependants(node));
    }

    return schedule;
}

ScheduledLayout ScheduleManager::parseScheduledLayout(const xml_node& layoutNode)
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

DefaultScheduledLayout ScheduleManager::parseDefaultLayout(const xml_node& layoutNode)
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

OverlayLayoutsList ScheduleManager::parseOverlayLayouts(const xml_node& overlaysNode)
{
    OverlayLayoutsList overlayLayouts;

    for(auto [name, node] : overlaysNode)
    {
        overlayLayouts.addLayout(parseScheduledLayout(node));
    }

    return overlayLayouts;
}

std::vector<std::string> ScheduleManager::parseDependants(const xml_node& dependantsNode)
{
    std::vector<std::string> dependants;

    for(auto&& [name, file] : dependantsNode)
    {
        dependants.emplace_back(file.get_value<std::string>());
    }

    return dependants;
}

void ScheduleManager::update(const std::string& scheduleXml)
{
    m_schedule = parseSchedule(Utils::parseXmlFromString(scheduleXml));

    std::ofstream out(m_path.string());
    out << scheduleXml;
}

LayoutSchedule ScheduleManager::schedule() const
{
    return m_schedule;
}
