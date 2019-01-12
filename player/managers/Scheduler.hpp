#pragma once

#include "events/Observable.hpp"
#include "xmds/Schedule.hpp"

#include <memory>
#include <vector>

class IMainLayout;

class Scheduler : public Observable<>
{
public:
    Scheduler() = default;
    void update(const Schedule::Result& schedule);
    std::unique_ptr<IMainLayout> nextLayout();

private:
    void resetSchedule();
    void fillScheduleItems(const std::vector<ScheduledLayout>& scheduledItems);
    int findMaxShowPriority(const std::vector<ScheduledLayout>& scheduledItems);
    int nextLayoutToPlayId();
    bool isLayoutOnSchedule(const ScheduledLayout& layout) const;
    size_t increaseLayoutIndex(std::size_t index) const;
    int nextValidLayoutId();

private:
    DefaultScheduledLayout m_defaultLayout;
    std::vector<ScheduledLayout> m_layouts;
    size_t m_nextLayoutIndex = 0;

};
