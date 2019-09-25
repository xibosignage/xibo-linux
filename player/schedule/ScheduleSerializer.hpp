#pragma once

#include "LayoutSchedule.hpp"
#include "constants.hpp"

class FilePath;

struct ScheduleSerializeException : public std::exception
{
    const char* what() const noexcept override;
};

class ScheduleSerializer
{
public:
    void scheduleTo(const LayoutSchedule& schedule, const FilePath& path);

private:
    void scheduleToImpl(const LayoutSchedule& schedule, const FilePath& path);

    ptree_node scheduledLayoutNode(const ScheduledLayout& layout);
    ptree_node overlaysNode(const LayoutList& overlays);
    ptree_node defaultLayoutNode(const DefaultScheduledLayout& layout);
    ptree_node dependantsNode(const LayoutDependants& dependants);
};
