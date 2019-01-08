#pragma once

#include "events/Observable.hpp"

#include <memory>
#include <vector>

class IMainLayout;

struct DefaultLayout
{
    int id;
    std::vector<std::string> dependants;
};

struct ScheduledLayout
{
    int scheduleId;
    int id;
    int priority;
    std::string startDT;
    std::string endDT;
    std::vector<std::string> dependants;
};

class Scheduler : public Observable<>
{
public:
    Scheduler();
    std::unique_ptr<IMainLayout> nextLayout();

private:
    std::unique_ptr<IMainLayout> createLayout();
    std::string getLayoutXlfPath();
    int getNextLayoutId();
    size_t getNextLayoutIndex();


private:
    DefaultLayout m_defaultLayout;
    std::vector<ScheduledLayout> m_layouts;
    size_t m_layoutToPlayIndex = 0;

};
