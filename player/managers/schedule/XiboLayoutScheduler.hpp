#pragma once

#include "LayoutSchedule.hpp"
#include "SchedulerStatus.hpp"

#include <memory>
#include <vector>

class XiboLayoutScheduler
{
public:
    void reloadSchedule(LayoutSchedule&& schedule);
    int nextLayoutId();
    std::vector<int> nextOverlayLayoutsIds();
    int currentLayoutId() const;

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
    int m_currentLayoutId = EmptyLayoutId;

};