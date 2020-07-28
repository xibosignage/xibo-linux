#pragma once

#include "schedule/LayoutSchedule.hpp"
#include "schedule/OverlayLayoutQueue.hpp"
#include "schedule/RegularLayoutQueue.hpp"
#include "schedule/SchedulerStatus.hpp"

#include "common/dt/Timer.hpp"
#include "common/storage/FileCache.hpp"

#include <boost/signals2/signal.hpp>

using SignalScheduleUpdated = boost::signals2::signal<void(const LayoutSchedule&)>;
using SignalLayoutsUpdated = boost::signals2::signal<void()>;

class Scheduler
{
public:
    Scheduler(const FileCache& fileCache);
    void reloadSchedule(LayoutSchedule&& schedule);
    void reloadQueue();

    LayoutId nextLayout() const;
    LayoutId currentLayoutId() const;
    OverlaysIds overlayLayouts() const;
    SchedulerStatus status() const;     // TODO tests
    int scheduleIdBy(LayoutId id) const;  // TODO tests

    SignalScheduleUpdated& scheduleUpdated();
    SignalLayoutsUpdated& layoutUpdated();
    SignalLayoutsUpdated& overlaysUpdated();

private:
    RegularLayoutQueue regularQueueFrom(const LayoutSchedule& schedule);
    void updateCurrentLayout(LayoutId id);

    OverlayLayoutQueue overlayQueueFrom(const LayoutSchedule& schedule);
    void updateCurrentOverlays(const OverlaysIds& ids);

    boost::optional<ScheduledLayout> layoutById(int id) const;

    void restartTimer();
    DateTime closestLayoutDt();

    bool layoutOnSchedule(const ScheduledLayout& layout) const;
    template <typename Layout>
    bool layoutValid(const Layout& layout) const;

    template <typename LayoutsList>
    void fillSchedulerStatus(SchedulerStatus& status, const LayoutsList& layouts) const;
    void addDefaultToStatus(SchedulerStatus& status, const DefaultScheduledLayout& layout) const;

private:
    const FileCache& fileCache_;
    boost::optional<LayoutSchedule> schedule_;
    RegularLayoutQueue regularQueue_;
    OverlayLayoutQueue overlayQueue_;
    SignalScheduleUpdated scheduleUpdated_;
    SignalLayoutsUpdated layoutUpdated_;
    SignalLayoutsUpdated overlaysUpdated_;
    Timer timer_;
};
