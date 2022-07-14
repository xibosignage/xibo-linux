#include "ScheduleParser.hpp"

#include "cms/xmds/Resources.hpp"
#include "common/parsing/Parsing.hpp"
#include "common/dt/DateTime.hpp"
#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"

#include <boost/core/ignore_unused.hpp>

namespace Resources = XmdsResources::Schedule;

LayoutSchedule ScheduleParser::scheduleFrom(const FilePath& path)
{
    try
    {
        if (!FileSystem::exists(path)) return {};

        return scheduleFromImpl(Parsing::xmlFrom(path));
    }
    catch (std::exception&)
    {
        throw ScheduleParser::Error{"Schedule is invalid"};
    }
}

LayoutSchedule ScheduleParser::scheduleFrom(const std::string& xmlSchedule)
{
    try
    {
        return scheduleFromImpl(Parsing::xmlFrom(xmlSchedule));
    }
    catch (std::exception&)
    {
        throw ScheduleParser::Error{"Schedule is invalid"};
    }
}

LayoutSchedule ScheduleParser::scheduleFromImpl(const XmlNode& scheduleXml)
{
    LayoutSchedule schedule;
    auto scheduleNode = scheduleXml.get_child(Resources::Schedule);

    schedule.generatedTime = DateTime::fromString(scheduleNode.get<std::string>(Resources::Layout::Generated));

    for (auto [name, node] : scheduleNode)
    {
        if (name == Resources::ScheduledLayout)
            schedule.regularLayouts.emplace_back(scheduledLayoutFrom(node));
        else if (name == Resources::DefaultLayout)
            schedule.defaultLayout = defaultLayoutFrom(node);
        else if (name == Resources::Overlays)
            schedule.overlayLayouts = overlayLayoutsFrom(node);
        else if (name == Resources::ScheduledCommand)
            schedule.commands.emplace_back(commandFrom(node));
        else if (name == Resources::GlobalDependants)
            schedule.globalDependants = dependantsFrom(node);
    }

    return schedule;
}

ScheduledLayout ScheduleParser::scheduledLayoutFrom(const XmlNode& node)
{
    ScheduledLayout layout;

    layout.scheduleId = node.get<int>(Resources::ScheduleId);
    layout.id = node.get<int>(Resources::Layout::Id);
    layout.startDT = DateTime::fromString(node.get<std::string>(Resources::Layout::StartDT));
    layout.endDT = DateTime::fromString(node.get<std::string>(Resources::Layout::EndDT));
    layout.priority = node.get<int>(Resources::Layout::Priority);

    if (auto dependants = node.get_child_optional(Resources::Layout::LocalDependants))
    {
        layout.dependants = dependantsFrom(dependants.value());
    }

    return layout;
}

DefaultScheduledLayout ScheduleParser::defaultLayoutFrom(const XmlNode& node)
{
    DefaultScheduledLayout layout;

    layout.id = node.get<int>(Resources::Layout::Id);
    if (auto dependants = node.get_child_optional(Resources::Layout::LocalDependants))
    {
        layout.dependants = dependantsFrom(dependants.value());
    }

    return layout;
}

LayoutList ScheduleParser::overlayLayoutsFrom(const XmlNode& overlaysNode)
{
    LayoutList overlayLayouts;

    for (auto [name, node] : overlaysNode)
    {
        boost::ignore_unused(name);
        overlayLayouts.emplace_back(scheduledLayoutFrom(node));
    }

    return overlayLayouts;
}

ScheduledCommand ScheduleParser::commandFrom(const XmlNode& commandNode)
{
    ScheduledCommand command;

    command.scheduleId = commandNode.get<int>(Resources::ScheduleId);
    command.startDT = DateTime::fromString(commandNode.get<std::string>(Resources::Command::StartDT));
    command.code = CommandCode{commandNode.get<std::string>(Resources::Command::Code)};

    return command;
}

LayoutDependants ScheduleParser::dependantsFrom(const XmlNode& dependantsNode)
{
    LayoutDependants dependants;

    for (auto&& [name, file] : dependantsNode)
    {
        boost::ignore_unused(name);
        dependants.emplace_back(file.get_value<std::string>());
    }

    return dependants;
}
