#pragma once

#include "constants.hpp"
#include "schedule/LayoutSchedule.hpp"

class FilePath;

class ScheduleSerializer
{
public:
    struct Error : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    void scheduleTo(const LayoutSchedule& schedule, const FilePath& path);

private:
    void scheduleToImpl(const LayoutSchedule& schedule, const FilePath& path);

    PtreeNode scheduledLayoutNode(const ScheduledLayout& layout);
    PtreeNode overlaysNode(const LayoutList& overlays);
    PtreeNode defaultLayoutNode(const DefaultScheduledLayout& layout);
    PtreeNode dependantsNode(const LayoutDependants& dependants);
};
