#include "ScheduleSerializer.hpp"

#include "common/Parsing.hpp"
#include "common/dt/DateTime.hpp"
#include "common/fs/FilePath.hpp"
#include "networking/xmds/Resources.hpp"  // TODO: remove dependency

#include <boost/property_tree/xml_parser.hpp>

// TODO: remove dependency
namespace Resources = XmdsResources::Schedule;

void ScheduleSerializer::scheduleTo(const LayoutSchedule& schedule, const FilePath& path)
{
    try
    {
        return scheduleToImpl(schedule, path);
    }
    catch (std::exception&)
    {
        throw ScheduleSerializer::Error{"Schedule serialization failed"};
    }
}

void ScheduleSerializer::scheduleToImpl(const LayoutSchedule& schedule, const FilePath& path)
{
    PtreeNode root;
    auto& scheduleNode = root.add_child(Resources::Schedule, {});
    scheduleNode.put(Resources::Generated, DateTime::toString(schedule.generatedTime));

    for (auto&& layout : schedule.regularLayouts)
    {
        scheduleNode.add_child(Resources::Layout, scheduledLayoutNode(layout));
    }

    scheduleNode.add_child(Resources::Overlays, overlaysNode(schedule.overlayLayouts));
    scheduleNode.add_child(Resources::GlobalDependants, dependantsNode(schedule.globalDependants));
    scheduleNode.add_child(Resources::DefaultLayout, defaultLayoutNode(schedule.defaultLayout));

    boost::property_tree::write_xml(path.string(), root);
}

PtreeNode ScheduleSerializer::scheduledLayoutNode(const ScheduledLayout& layout)
{
    PtreeNode node;

    node.put(Resources::Id, layout.id);
    node.put(Resources::StartDT, DateTime::toString(layout.startDT));
    node.put(Resources::EndDT, DateTime::toString(layout.endDT));
    node.put(Resources::ScheduleId, layout.scheduleId);
    node.put(Resources::Priority, layout.priority);
    node.add_child(Resources::LocalDependants, dependantsNode(layout.dependants));

    return node;
}

PtreeNode ScheduleSerializer::overlaysNode(const LayoutList& overlays)
{
    PtreeNode node;

    for (auto&& layout : overlays)
    {
        node.add_child(Resources::OverlayLayout, scheduledLayoutNode(layout));
    }

    return node;
}

PtreeNode ScheduleSerializer::defaultLayoutNode(const DefaultScheduledLayout& layout)
{
    PtreeNode node;

    node.put(Resources::Id, layout.id);
    node.add_child(Resources::LocalDependants, dependantsNode(layout.dependants));

    return node;
}

PtreeNode ScheduleSerializer::dependantsNode(const LayoutDependants& dependants)
{
    PtreeNode node;

    for (auto&& dependant : dependants)
    {
        node.add(Resources::DependantFile, dependant);
    }

    return node;
}
