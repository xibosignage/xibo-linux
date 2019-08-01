#pragma once

#include "LayoutSchedule.hpp"
#include "IScheduleSerializer.hpp"
#include "common/fs/FilePath.hpp"

#include <sigc++/signal.h>

using SignalScheduleAvailable = sigc::signal<void(const LayoutSchedule&)>;

class ScheduleManager
{
public:
    ScheduleManager(std::unique_ptr<IScheduleSerializer>&& serializer);

    void load(const FilePath& path);
    void update(const std::string& schedule);

    SignalScheduleAvailable scheduleUpdated();
    LayoutSchedule schedule() const;

private:
    void writeToFile(const std::string& scheduleXml);

private:
    std::unique_ptr<IScheduleSerializer> m_serializer;
    LayoutSchedule m_cachedSchedule;
    SignalScheduleAvailable m_scheduleUpdated;
    FilePath m_path;

};
