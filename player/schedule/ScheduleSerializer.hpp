#pragma once

#include "common/parsing/Parsing.hpp"
#include "common/PlayerRuntimeError.hpp"
#include "schedule/LayoutSchedule.hpp"

class FilePath;

class ScheduleSerializer
{
public:
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    void scheduleTo(const LayoutSchedule& schedule, const FilePath& path);

private:
    void scheduleToImpl(const LayoutSchedule& schedule, const FilePath& path);

    XmlNode scheduledLayoutNode(const ScheduledLayout& layout);
    XmlNode overlaysNode(const LayoutList& overlays);
    XmlNode defaultLayoutNode(const DefaultScheduledLayout& layout);
    XmlNode dependantsNode(const LayoutDependants& dependants);
};
