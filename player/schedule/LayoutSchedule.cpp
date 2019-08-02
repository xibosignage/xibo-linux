#include "LayoutSchedule.hpp"
#include "ScheduleSerializer.hpp"

#include "common/Utils.hpp"
#include "common/fs/FileSystem.hpp"

bool operator==(const LayoutSchedule& first, const LayoutSchedule& second)
{
    if(first.generatedTime != second.generatedTime) return false;
    if(first.defaultLayout != second.defaultLayout) return false;

    if(!Utils::containersEqual(first.regularLayouts, second.regularLayouts)) return false;
    if(!Utils::containersEqual(first.overlayLayouts, second.overlayLayouts)) return false;
    if(!Utils::containersEqual(first.globalDependants, second.globalDependants)) return false;

    return true;
}

bool operator!=(const LayoutSchedule& first, const LayoutSchedule& second)
{
    return !(first == second);
}

void LayoutSchedule::loadFrom(const FilePath& path)
{
    ScheduleSerializer serializer;

    *this = serializer.scheduleFrom(path);
}

void LayoutSchedule::loadFrom(const std::string& xmlString)
{
    ScheduleSerializer serializer;

    *this = serializer.scheduleFrom(xmlString);
}

void LayoutSchedule::saveTo(const FilePath& path)
{
    ScheduleSerializer serializer;

    serializer.scheduleTo(*this, path);
}
