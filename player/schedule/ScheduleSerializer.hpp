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

    PtreeNode scheduledLayoutNode(const ScheduledLayout& layout);
    PtreeNode overlaysNode(const LayoutList& overlays);
    PtreeNode defaultLayoutNode(const DefaultScheduledLayout& layout);
    PtreeNode dependantsNode(const LayoutDependants& dependants);
};
