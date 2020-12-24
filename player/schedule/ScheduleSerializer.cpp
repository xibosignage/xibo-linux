#include "ScheduleSerializer.hpp"

#include "cms/xmds/Resources.hpp"  // TODO: remove dependency
#include "common/parsing/Parsing.hpp"
#include "common/dt/DateTime.hpp"
#include "common/fs/FilePath.hpp"

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
    scheduleNode.put(Resources::Generated, schedule.generatedTime.string());

    for (auto&& layout : schedule.regularLayouts)
    {
        scheduleNode.add_child(Resources::Layout, scheduledLayoutNode(layout));
    }

    scheduleNode.add_child(Resources::Overlays, overlaysNode(schedule.overlayLayouts));
    scheduleNode.add_child(Resources::GlobalDependants, dependantsNode(schedule.globalDependants));
    scheduleNode.add_child(Resources::DefaultLayout, defaultLayoutNode(schedule.defaultLayout));

    Parsing::xmlTreeToFile(path, root);
}

XmlNode ScheduleSerializer::scheduledLayoutNode(const ScheduledLayout& layout)
{
    XmlNode node;

    node.put(Resources::Id, layout.id);
    node.put(Resources::StartDT, layout.startDT.string());
    node.put(Resources::EndDT, layout.endDT.string());
    node.put(Resources::ScheduleId, layout.scheduleId);
    node.put(Resources::Priority, layout.priority);
    node.add_child(Resources::LocalDependants, dependantsNode(layout.dependants));

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

    node.put(Resources::Id, layout.id);
    node.add_child(Resources::LocalDependants, dependantsNode(layout.dependants));

    return node;
}

XmlNode ScheduleSerializer::dependantsNode(const LayoutDependants& dependants)
{
    XmlNode node;

    for (auto&& dependant : dependants)
    {
        node.add(Resources::DependantFile, dependant);
    }

    return node;
}
