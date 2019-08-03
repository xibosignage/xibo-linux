#pragma once

#include "LayoutSchedule.hpp"
#include "SchedulerStatus.hpp"
#include "common/fs/FilePath.hpp"

#include <memory>
#include <vector>
#include <sigc++/signal.h>

using SignalScheduleAvailable = sigc::signal<void(const LayoutSchedule&)>;
class FileCache;

class Scheduler
{
public:
    Scheduler(const FileCache& fileCache);
    void reloadSchedule(LayoutSchedule&& schedule);

    int nextLayoutId();
    std::vector<int> nextOverlayLayoutsIds();
    int currentLayoutId() const;

    SignalScheduleAvailable scheduleUpdated();
    SchedulerStatus status();

private:
    int nextScheduledLayoutId();

    template<typename LayoutsList>
    void collectLayoutListStatus(SchedulerStatus& status, const LayoutsList& layouts);

    bool isLayoutOnSchedule(const ScheduledLayout& layout) const;
    bool isLayoutValid(const std::vector<std::string>& dependants) const;
    size_t increaseLayoutIndex(std::size_t index) const;

private:
    const FileCache& m_fileCache;
    LayoutSchedule m_schedule;
    SignalScheduleAvailable m_scheduleUpdated;
    int m_currentLayoutId = EmptyLayoutId;

};
