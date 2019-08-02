#pragma once

#include "LayoutSchedule.hpp"
#include "SchedulerStatus.hpp"
#include "common/fs/FilePath.hpp"

#include <memory>
#include <vector>
#include <sigc++/signal.h>

using SignalScheduleAvailable = sigc::signal<void(const LayoutSchedule&)>;

class LayoutScheduler
{
public:
    void scheduleFrom(const FilePath& file);
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
    LayoutSchedule m_schedule;
    FilePath m_filePath;
    SignalScheduleAvailable m_scheduleUpdated;
    int m_currentLayoutId = EmptyLayoutId;

};
