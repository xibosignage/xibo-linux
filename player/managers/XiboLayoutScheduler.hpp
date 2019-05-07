#pragma once

#include "LayoutSchedule.hpp"
#include "SchedulerStatus.hpp"
#include "networking/xmds/Schedule.hpp"

#include <memory>
#include <vector>

class XiboLayoutScheduler
{
public:
    void update(const Schedule::Result& schedule);
    int nextLayoutId();
    int currentLayoutId() const;

    SchedulerStatus status();

private:
    int nextScheduledLayoutId();

    bool isLayoutOnSchedule(const ScheduledLayout& layout) const;
    bool isLayoutValid(const std::vector<std::string>& dependants) const;
    size_t increaseLayoutIndex(std::size_t index) const;

    std::vector<int> collectValidLayouts();
    std::vector<int> collectInvalidLayouts();
    std::vector<int> collectScheduledLayouts();

private:
    LayoutSchedule m_schedule;
    int m_currentLayoutId = EMPTY_LAYOUT_ID;

};
