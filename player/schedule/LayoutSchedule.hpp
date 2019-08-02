#pragma once

#include "ScheduleItem.hpp"

class FilePath;

struct LayoutSchedule
{
    void loadFrom(const FilePath& path);
    void loadFrom(const std::string& xmlString);
    void saveTo(const FilePath& path);

    DateTime generatedTime;
    LayoutDependants globalDependants;
    LayoutList regularLayouts;
    LayoutList overlayLayouts;
    DefaultScheduledLayout defaultLayout;
};

bool operator== (const LayoutSchedule& first, const LayoutSchedule& second);
bool operator!= (const LayoutSchedule& first, const LayoutSchedule& second);
