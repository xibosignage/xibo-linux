#pragma once

#include "events/Observable.hpp"

#include <memory>
#include <vector>

class IMainLayout;
class IFileSystemAdaptor;

class Scheduler : public Observable<>
{
public:
    Scheduler();
    void update(const LayoutSchedule& schedule);
    std::unique_ptr<IMainLayout> nextLayout();

private:
    void resetSchedule();
    void fillScheduleItems(const std::vector<ScheduledLayout>& scheduledItems);
    int findMaxShowPriority(const std::vector<ScheduledLayout>& scheduledItems);
    int nextLayoutToPlayId();
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
