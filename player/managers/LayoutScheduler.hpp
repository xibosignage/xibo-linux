#pragma once

#include "LayoutSchedule.hpp"

#include <memory>
#include <vector>

class LayoutScheduler
{
public:
    LayoutScheduler() = default;
    void update(const LayoutSchedule& schedule);
    int nextLayoutId();
    int currentLayoutId() const;

private:
    void resetSchedule();
    void fillScheduleItems(const std::vector<ScheduledLayout>& scheduledItems);
    int findMaxShowPriority(const std::vector<ScheduledLayout>& scheduledItems);
    bool isLayoutOnSchedule(const ScheduledLayout& layout) const;
    bool allFilesExist(const std::vector<std::string>& dependants) const;
    size_t increaseLayoutIndex(std::size_t index) const;
    int nextValidLayoutId();

private:
    DefaultScheduledLayout m_defaultLayout;
    std::vector<ScheduledLayout> m_layouts;
    std::vector<std::string> m_globalDependants;
    size_t m_nextLayoutIndex = 0;
    int m_layoutId = DEFAULT_LAYOUT_ID;

};
