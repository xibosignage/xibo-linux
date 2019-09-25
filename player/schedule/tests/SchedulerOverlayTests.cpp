#include "Scheduler.hpp"
#include "SchedulerLayoutTests.hpp"

static void addToQueue(LayoutSchedule& schedule, ScheduledLayout&& type)
{
    schedule.overlayLayouts.emplace_back(std::move(type));
}

TEST_F(SchedulerLayoutTests, NoOverlayLayoutsDefaultLayoutInvalid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), std::vector<int>{});
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsSamePriorityAllNotInCache)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, notInCacheLayout(DefaultId + i, DefaultPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), std::vector<int>{});
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsSamePriorityAllNotInRange)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, notInRangeLayout(DefaultId + i, DefaultPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), std::vector<int>{});
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsSamePriorityAllValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, validLayout(DefaultId + i, DefaultPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultId + 1, DefaultId + 2, DefaultId + 3}));
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsSamePrioritySomeNotInCache)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    addToQueue(schedule, validLayout(DefaultId + 1, DefaultPriority));
    addToQueue(schedule, notInCacheLayout(DefaultId + 2, DefaultPriority));
    addToQueue(schedule, validLayout(DefaultId + 3, DefaultPriority));
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultId + 1, DefaultId + 3}));
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsSamePrioritySomeNotInRange)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    addToQueue(schedule, validLayout(DefaultId + 1, DefaultPriority));
    addToQueue(schedule, notInRangeLayout(DefaultId + 2, DefaultPriority));
    addToQueue(schedule, validLayout(DefaultId + 3, DefaultPriority));
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultId + 1, DefaultId + 3}));
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsDifferentPrioritiesAllValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, validLayout(DefaultId + i, DefaultPriority + i));
    }
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), std::vector{DefaultId + 3});
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsDifferentPrioritiesSomeInvalid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    addToQueue(schedule, validLayout(DefaultId + 1, DefaultPriority + 1));
    addToQueue(schedule, notInRangeLayout(DefaultId + 2, DefaultPriority + 2));
    addToQueue(schedule, notInCacheLayout(DefaultId + 3, DefaultPriority + 3));
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), std::vector{DefaultId + 1});
}
