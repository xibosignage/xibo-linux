#include "Scheduler.hpp"

#include "common/dt/DateTime.hpp"
#include "common/logger/Logging.hpp"

Scheduler::Scheduler(const FileCache& fileCache) : fileCache_{fileCache}, schedule_{} {}

void Scheduler::reloadSchedule(LayoutSchedule&& schedule)
{
    if (!schedule_.has_value() || schedule_ != schedule)
    {
        schedule_ = std::move(schedule);
        scheduleUpdated_(schedule_.value());

        reloadQueue();
    }
}

void Scheduler::reloadQueue()
{
    assert(schedule_);

    auto current = currentLayoutId();
    auto overlays = overlayLayouts();

    regularQueue_ = regularQueueFrom(schedule_.value());
    overlayQueue_ = overlayQueueFrom(schedule_.value());
    restartTimer();

    updateCurrentLayout(current);
    updateCurrentOverlays(overlays);
}

RegularLayoutQueue Scheduler::regularQueueFrom(const LayoutSchedule& schedule)
{
    RegularLayoutQueue queue;

    for (auto&& layout : schedule.regularLayouts)
    {
        if (layoutOnSchedule(layout) && layoutValid(layout))
        {
            queue.add(layout);
        }
    }

    if (layoutValid(schedule.defaultLayout))
    {
        queue.addDefault(schedule.defaultLayout);
    }

    return queue;
}

OverlayLayoutQueue Scheduler::overlayQueueFrom(const LayoutSchedule& schedule)
{
    OverlayLayoutQueue queue;

    for (auto&& layout : schedule.overlayLayouts)
    {
        if (layoutOnSchedule(layout) && layoutValid(layout))
        {
            queue.add(layout);
        }
    }

    return queue;
}

void Scheduler::updateCurrentLayout(LayoutId id)
{
    if (regularQueue_.inQueue(id))
    {
        regularQueue_.updateCurrent(id);
    }
    else
    {
        layoutUpdated_();
    }
}

void Scheduler::updateCurrentOverlays(const OverlaysIds& ids)
{
    if (overlayQueue_.overlays() != ids)
    {
        overlaysUpdated_();
    }
}

bool Scheduler::layoutOnSchedule(const ScheduledLayout& layout) const
{
    auto currentDT = DateTime::now();

    if (currentDT >= layout.startDT && currentDT < layout.endDT)
    {
        return true;
    }

    return false;
}

template <typename Layout>
bool Scheduler::layoutValid(const Layout& layout) const
{
    assert(schedule_);

    auto layoutFile = std::to_string(layout.id) + ".xlf";
    if (!fileCache_.valid(layoutFile)) return false;

    for (auto&& dependant : layout.dependants)
    {
        if (!fileCache_.valid(dependant))
        {
            return false;
        }
    }

    for (auto&& dependant : schedule_->globalDependants)
    {
        if (!fileCache_.valid(dependant))
        {
            return false;
        }
    }

    return true;
}

LayoutId Scheduler::nextLayout() const
{
    return regularQueue_.next();
}

LayoutId Scheduler::currentLayoutId() const
{
    return regularQueue_.current();
}

OverlaysIds Scheduler::overlayLayouts() const
{
    return overlayQueue_.overlays();
}

SignalScheduleUpdated& Scheduler::scheduleUpdated()
{
    return scheduleUpdated_;
}

SignalLayoutsUpdated& Scheduler::overlaysUpdated()
{
    return overlaysUpdated_;
}

DateTime Scheduler::closestLayoutDt()
{
    assert(schedule_);

    auto now = DateTime::now();
    DateTime closestDt;

    for (auto&& layout : schedule_->regularLayouts)
    {
        if (now < layout.startDT && layout.startDT < closestDt) closestDt = layout.startDT;
        if (now < layout.endDT && layout.endDT < closestDt) closestDt = layout.endDT;
    }

    for (auto&& layout : schedule_->overlayLayouts)
    {
        if (now < layout.startDT && layout.startDT < closestDt) closestDt = layout.startDT;
        if (now < layout.endDT && layout.endDT < closestDt) closestDt = layout.endDT;
    }

    return closestDt;
}

void Scheduler::restartTimer()
{
    auto dt = closestLayoutDt();
    auto duration = (dt - DateTime::now()).total_seconds();

    if (dt.valid() && duration > 0)
    {
        Log::trace("[Scheduler] Timer restarted: {}", duration);

        timer_.startOnce(std::chrono::seconds(duration), std::bind(&Scheduler::reloadQueue, this));
    }
}

SignalLayoutsUpdated& Scheduler::layoutUpdated()
{
    return layoutUpdated_;
}

SchedulerStatus Scheduler::status() const
{
    assert(schedule_);

    SchedulerStatus status;

    fillSchedulerStatus(status, schedule_->regularLayouts);
    fillSchedulerStatus(status, schedule_->overlayLayouts);
    addDefaultToStatus(status, schedule_->defaultLayout);

    status.generatedTime = schedule_->generatedTime.string();
    status.currentLayout = currentLayoutId();

    return status;
}

int Scheduler::scheduleIdBy(LayoutId id) const
{
    assert(schedule_);

    auto layout = layoutById(id);
    if (layout)
    {
        return layout->scheduleId;
    }
    return DefaultScheduleId;
}

boost::optional<ScheduledLayout> Scheduler::layoutById(int id) const
{
    assert(schedule_);

    {
        auto&& regularLayouts = schedule_->regularLayouts;
        auto it = std::find_if(regularLayouts.begin(), regularLayouts.end(), [id](const ScheduledLayout& other) {
            return other.id == id;
        });

        if (it != regularLayouts.end()) return *it;
    }
    {
        auto&& overlayLayouts = schedule_->overlayLayouts;
        auto it = std::find_if(overlayLayouts.begin(), overlayLayouts.end(), [id](const ScheduledLayout& other) {
            return other.id == id;
        });

        if (it != overlayLayouts.end()) return *it;
    }

    return {};
}

template <typename LayoutsList>
void Scheduler::fillSchedulerStatus(SchedulerStatus& status, const LayoutsList& layouts) const
{
    for (auto&& layout : layouts)
    {
        if (layoutValid(layout))
        {
            status.validLayouts.emplace_back(layout.id);
            if (layoutOnSchedule(layout))
            {
                status.scheduledLayouts.emplace_back(layout.id);
            }
        }
        else
        {
            status.invalidLayouts.emplace_back(layout.id);
        }
    }
}

void Scheduler::addDefaultToStatus(SchedulerStatus& status, const DefaultScheduledLayout& layout) const
{
    if (layoutValid(layout))
    {
        status.validLayouts.emplace_back(layout.id);
    }
    else
    {
        status.invalidLayouts.emplace_back(layout.id);
    }
}
