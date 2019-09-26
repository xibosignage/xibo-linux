#pragma once

#include "LayoutSchedule.hpp"
#include "OverlayLayoutQueue.hpp"
#include "RegularLayoutQueue.hpp"
#include "SchedulerStatus.hpp"

#include "common/dt/Timer.hpp"
#include "common/fs/IFileCache.hpp"

#include <boost/signals2/signal.hpp>

using SignalScheduleUpdated = boost::signals2::signal<void(const LayoutSchedule&)>;
using SignalLayoutsUpdated = boost::signals2::signal<void()>;

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

    SignalScheduleUpdated& scheduleUpdated();
    SignalLayoutsUpdated& layoutUpdated();
    SignalLayoutsUpdated& overlaysUpdated();

private:
    RegularLayoutQueue regularQueueFrom(const LayoutSchedule& schedule);
    void updateCurrentLayout(LayoutId id);

    OverlayLayoutQueue overlayQueueFrom(const LayoutSchedule& schedule);
    void updateCurrentOverlays(const OverlaysIds& ids);

    void restartTimer();
    DateTime closestLayoutDt();

    bool layoutOnSchedule(const ScheduledLayout& layout) const;
    template <typename Layout>
    bool layoutValid(const Layout& layout) const;

    template <typename LayoutsList>
    void schedulerStatus(SchedulerStatus& status, const LayoutsList& layouts) const;

private:
    const IFileCache& m_fileCache;
    LayoutSchedule m_schedule;
    RegularLayoutQueue m_regularQueue;
    OverlayLayoutQueue m_overlayQueue;
    SignalScheduleUpdated m_scheduleUpdated;
    SignalLayoutsUpdated m_layoutUpdated;
    SignalLayoutsUpdated m_overlaysUpdated;
    Timer m_timer;
};
