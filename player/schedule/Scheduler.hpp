#pragma once

#include "LayoutSchedule.hpp"
#include "SchedulerStatus.hpp"
#include "OverlayLayoutQueue.hpp"
#include "RegularLayoutQueue.hpp"

#include "common/fs/IFileCache.hpp"

#include <sigc++/signal.h>

using SignalScheduleAvailable = sigc::signal<void(const LayoutSchedule&)>;
using SignalLayoutInterrupted = sigc::signal<void()>;

class Scheduler
{
public:
    Scheduler(const IFileCache& fileCache);
    void reloadSchedule(LayoutSchedule&& schedule);
    void reloadQueue();

    int nextLayout();
    std::vector<int> nextOverlayLayouts();
    int currentLayoutId() const;

    SignalScheduleAvailable scheduleUpdated();
    SignalLayoutInterrupted layoutInterrupted();
    SchedulerStatus status();

private:
    void updateInQueue(int id);
    RegularLayoutQueue regularQueueFrom(const LayoutSchedule& schedule);
    OverlayLayoutQueue overlayQueueFrom(const LayoutSchedule& schedule);

    template<typename LayoutsList> void schedulerStatus(SchedulerStatus& status, const LayoutsList& layouts);

    bool layoutOnSchedule(const ScheduledLayout& layout) const;
    template<typename Layout> bool layoutValid(const Layout& layout) const;

private:
    const IFileCache& m_fileCache;
    LayoutSchedule m_schedule;
    RegularLayoutQueue m_regularQueue;
    OverlayLayoutQueue m_overlayQueue;
    SignalScheduleAvailable m_scheduleUpdated;
    SignalLayoutInterrupted m_layoutInterrupted;
    int m_currentLayoutId = EmptyLayoutId;

};
