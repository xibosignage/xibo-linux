#pragma once

#include "events/EventPublisher.hpp"
#include "LayoutSchedule.hpp"

#include <memory>
#include <vector>

class IMainLayout;
class IFileSystemAdaptor;

class LayoutScheduler : public EventPublisher<>
{
public:
    LayoutScheduler();
    void update(const LayoutSchedule& schedule);
    int nextLayoutId();

private:
    void resetSchedule();
    void fillScheduleItems(const std::vector<ScheduledLayout>& scheduledItems);
    int findMaxShowPriority(const std::vector<ScheduledLayout>& scheduledItems);
    bool isLayoutOnSchedule(const ScheduledLayout& layout) const;
    bool allFilesExist(const std::vector<std::string>& dependants) const;
    size_t increaseLayoutIndex(std::size_t index) const;
    int nextValidLayoutId();

private:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    DefaultScheduledLayout m_defaultLayout;
    std::vector<ScheduledLayout> m_layouts;
    std::vector<std::string> m_globalDependants;
    size_t m_nextLayoutIndex = 0;

};
