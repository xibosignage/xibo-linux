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
    std::unique_ptr<IMainLayout> createLayout();
    std::string getLayoutXlfPath();
    int getNextLayoutId();
    size_t getNextLayoutIndex();

private:
    DefaultScheduledLayout m_defaultLayout;
    std::vector<ScheduledLayout> m_layouts;
    size_t m_layoutToPlayIndex = 0;

};
