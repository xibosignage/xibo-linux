#include "LayoutSchedule.hpp"

#include "schedule/ScheduleParser.hpp"
#include "schedule/ScheduleSerializer.hpp"

#include "common/Utils.hpp"
#include "common/fs/FilePath.hpp"

LayoutSchedule LayoutSchedule::fromFile(const FilePath& path)
{
    ScheduleParser parser;
    return parser.scheduleFrom(path);
}

LayoutSchedule LayoutSchedule::fromString(const std::string& string)
{
    ScheduleParser parser;
    return parser.scheduleFrom(string);
}

void LayoutSchedule::toFile(const FilePath& path) const
{
    ScheduleSerializer serializer;
    serializer.scheduleTo(*this, path);
}

bool operator==(const LayoutSchedule& first, const LayoutSchedule& second)
{
    if (first.generatedTime != second.generatedTime) return false;
    if (first.defaultLayout != second.defaultLayout) return false;

    if (!Utils::containersEqual(first.regularLayouts, second.regularLayouts)) return false;
    if (!Utils::containersEqual(first.overlayLayouts, second.overlayLayouts)) return false;
    if (!Utils::containersEqual(first.globalDependants, second.globalDependants)) return false;

    return true;
}

bool operator!=(const LayoutSchedule& first, const LayoutSchedule& second)
{
    return !(first == second);
}
