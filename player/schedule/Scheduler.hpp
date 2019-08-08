#pragma once

#include "LayoutSchedule.hpp"
#include "SchedulerStatus.hpp"
#include "OverlayLayoutQueue.hpp"
#include "RegularLayoutQueue.hpp"

#include "common/fs/IFileCache.hpp"

#include <sigc++/signal.h>

using SignalScheduleAvailable = sigc::signal<void(const LayoutSchedule&)>;
using SignalLayoutsUpdated = sigc::signal<void()>;

class Scheduler
{
public:
    Scheduler(const IFileCache& fileCache);
    void reloadSchedule(LayoutSchedule&& schedule);
    void reloadQueue();

    LayoutId nextLayout() const;
    LayoutId currentLayoutId() const;
    OverlaysIds overlayLayouts() const;
    SchedulerStatus status() const;

    SignalScheduleAvailable scheduleUpdated();
    SignalLayoutsUpdated layoutUpdated();
    SignalLayoutsUpdated overlaysUpdated();

private:
    void updateCurrentLayout(LayoutId id);
    void updateCurrentOverlays(const OverlaysIds& ids);
    RegularLayoutQueue regularQueueFrom(const LayoutSchedule& schedule);
    OverlayLayoutQueue overlayQueueFrom(const LayoutSchedule& schedule);

    template<typename LayoutsList> void schedulerStatus(SchedulerStatus& status, const LayoutsList& layouts) const;

    bool layoutOnSchedule(const ScheduledLayout& layout) const;
    template<typename Layout> bool layoutValid(const Layout& layout) const;

private:
    const IFileCache& m_fileCache;
    LayoutSchedule m_schedule;
    RegularLayoutQueue m_regularQueue;
    OverlayLayoutQueue m_overlayQueue;
    SignalScheduleAvailable m_scheduleUpdated;
    SignalLayoutsUpdated m_layoutUpdated;
    SignalLayoutsUpdated m_overlaysUpdated;
};
