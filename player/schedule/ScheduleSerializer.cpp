#include "ScheduleSerializer.hpp"

#include "cms/xmds/Resources.hpp"  // TODO: remove dependency
#include "common/dt/DateTime.hpp"
#include "common/fs/FilePath.hpp"
#include "common/parsing/Parsing.hpp"

namespace Resources = XmdsResources::Schedule;

void ScheduleSerializer::scheduleTo(const LayoutSchedule& schedule, const FilePath& path)
{
    try
    {
        return scheduleToImpl(schedule, path);
    }
    catch (std::exception&)
    {
        throw ScheduleSerializer::Error{"ScheduleSerializer", "Schedule serialization failed"};
    }
}

void ScheduleSerializer::scheduleToImpl(const LayoutSchedule& schedule, const FilePath& path)
{
    XmlNode root;
    auto& scheduleNode = root.add_child(Resources::Schedule, {});
    scheduleNode.put(Resources::Layout::Generated, schedule.generatedTime.string());

    for (auto&& layout : schedule.regularLayouts)
    {
        scheduleNode.add_child(Resources::ScheduledLayout, scheduledLayoutNode(layout));
    }

    for (auto&& command : schedule.commands)
    {
        scheduleNode.add_child(Resources::ScheduledCommand, commandNode(command));
    }

    scheduleNode.add_child(Resources::Overlays, overlaysNode(schedule.overlayLayouts));
    scheduleNode.add_child(Resources::GlobalDependants, dependantsNode(schedule.globalDependants));
    scheduleNode.add_child(Resources::DefaultLayout, defaultLayoutNode(schedule.defaultLayout));

    Parsing::xmlTreeToFile(path, root);
}

XmlNode ScheduleSerializer::commandNode(const ScheduledCommand& command)
{
    XmlNode node;

    node.put(Resources::ScheduleId, command.scheduleId);
    node.put(Resources::Command::StartDT, command.startDT.string());
    node.put(Resources::Command::Code, static_cast<std::string>(command.code));

    return node;
}

XmlNode ScheduleSerializer::scheduledLayoutNode(const ScheduledLayout& layout)
{
    XmlNode node;

    node.put(Resources::Layout::Id, layout.id);
    node.put(Resources::Layout::StartDT, layout.startDT.string());
    node.put(Resources::Layout::EndDT, layout.endDT.string());
    node.put(Resources::ScheduleId, layout.scheduleId);
    node.put(Resources::Layout::Priority, layout.priority);
    node.add_child(Resources::Layout::LocalDependants, dependantsNode(layout.dependants));

    return node;
}

XmlNode ScheduleSerializer::overlaysNode(const LayoutList& overlays)
{
    XmlNode node;

    for (auto&& layout : overlays)
    {
        node.add_child(Resources::OverlayLayout, scheduledLayoutNode(layout));
    }

    return node;
}

XmlNode ScheduleSerializer::defaultLayoutNode(const DefaultScheduledLayout& layout)
{
    XmlNode node;

    node.put(Resources::Layout::Id, layout.id);
    node.add_child(Resources::Layout::LocalDependants, dependantsNode(layout.dependants));

    return node;
}

XmlNode ScheduleSerializer::dependantsNode(const LayoutDependants& dependants)
{
    XmlNode node;

    for (auto&& dependant : dependants)
    {
        node.add(Resources::Layout::DependantFile, dependant);
    }

    return node;
}
