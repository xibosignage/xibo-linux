#pragma once

#include "ParsedLayoutSchedule.hpp"

class FilePath;

class IScheduleSerializer
{
public:
    virtual ~IScheduleSerializer() = default;

    virtual LayoutSchedule parseSchedule(const FilePath& path) = 0;
    virtual LayoutSchedule parseSchedule(const std::string& xmlSchedule) = 0;
};
