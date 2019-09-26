#include "ScheduleSerializer.hpp"

#include "common/Parsing.hpp"
#include "common/dt/DateTime.hpp"
#include "common/fs/FilePath.hpp"
#include "networking/xmds/Resources.hpp"

#include <boost/property_tree/xml_parser.hpp>

namespace Resources = XmdsResources::Schedule;

const char* ScheduleSerializeException::what() const noexcept
{
    return "Schedule serialize failed";
}

void ScheduleSerializer::scheduleTo(const LayoutSchedule& schedule, const FilePath& path)
{
    try
    {
        return scheduleToImpl(schedule, path);
    }
    catch (std::exception&)
    {
        throw ScheduleSerializeException{};
    }
}

void ScheduleSerializer::scheduleToImpl(const LayoutSchedule& schedule, const FilePath& path)
{
    ptree_node root;
    auto& scheduleNode = root.add_child(Resources::Schedule, {});
    scheduleNode.put(Resources::Generated, DateTime::toString(schedule.generatedTime));

    for (auto&& layout : schedule.regularLayouts)
    {
        scheduleNode.add_child(Resources::Layout, scheduledLayoutNode(layout));
    }

    scheduleNode.add_child(Resources::Overlays, overlaysNode(schedule.overlayLayouts));
    scheduleNode.add_child(Resources::GlobalDependants, dependantsNode(schedule.globalDependants));
    scheduleNode.add_child(Resources::DefaultLayout, defaultLayoutNode(schedule.defaultLayout));

    boost::property_tree::write_xml(path, root);
}

ptree_node ScheduleSerializer::scheduledLayoutNode(const ScheduledLayout& layout)
{
    ptree_node node;

    node.put(Resources::Id, layout.id);
    node.put(Resources::StartDT, DateTime::toString(layout.startDT));
    node.put(Resources::EndDT, DateTime::toString(layout.endDT));
    node.put(Resources::ScheduleId, layout.scheduleId);
    node.put(Resources::Priority, layout.priority);
    node.add_child(Resources::LocalDependants, dependantsNode(layout.dependants));

    return node;
}

ptree_node ScheduleSerializer::overlaysNode(const LayoutList& overlays)
{
    ptree_node node;

    for (auto&& layout : overlays)
    {
        node.add_child(Resources::OverlayLayout, scheduledLayoutNode(layout));
    }

    return node;
}

ptree_node ScheduleSerializer::defaultLayoutNode(const DefaultScheduledLayout& layout)
{
    ptree_node node;

    node.put(Resources::Id, layout.id);
    node.add_child(Resources::LocalDependants, dependantsNode(layout.dependants));

    return node;
}

ptree_node ScheduleSerializer::dependantsNode(const LayoutDependants& dependants)
{
    ptree_node node;

    for (auto&& dependant : dependants)
    {
        node.add(Resources::DependantFile, dependant);
    }

    return node;
}
