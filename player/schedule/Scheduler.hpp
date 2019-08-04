#pragma once

#include "LayoutSchedule.hpp"
#include "SchedulerStatus.hpp"
#include "QueueTypes.hpp"

#include "common/fs/IFileCache.hpp"

#include <sigc++/signal.h>

using SignalScheduleAvailable = sigc::signal<void(const LayoutSchedule&)>;

class Scheduler
{
public:
    Scheduler(const IFileCache& fileCache);
    void reloadSchedule(LayoutSchedule&& schedule);

    int nextLayoutId();
    std::vector<int> nextOverlayLayoutsIds();
    int currentLayoutId() const;

    SignalScheduleAvailable scheduleUpdated();
    SchedulerStatus status();

private:
    RegularLayoutQueue regularQueueFrom(const LayoutSchedule& schedule);
    OverlayLayoutQueue overlayQueueFrom(const LayoutSchedule& schedule);

    template<typename LayoutsList> void collectLayoutListStatus(SchedulerStatus& status, const LayoutsList& layouts);

    bool isLayoutOnSchedule(const ScheduledLayout& layout) const;
    template<typename Layout> bool isLayoutValid(const Layout& layout) const;

private:
    const IFileCache& m_fileCache;
    LayoutSchedule m_schedule;
    RegularLayoutQueue m_regularQueue;
    OverlayLayoutQueue m_overlayQueue;
    SignalScheduleAvailable m_scheduleUpdated;
    int m_currentLayoutId = EmptyLayoutId;

};
