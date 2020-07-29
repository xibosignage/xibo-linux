#include "schedule/Scheduler.hpp"
#include "schedule/tests/SchedulerLayoutTests.hpp"

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
        addToQueue(schedule, notInCacheLayout(DefaultTestId + i, DefaultTestPriority));
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
        addToQueue(schedule, notInRangeLayout(DefaultTestId + i, DefaultTestPriority));
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
        addToQueue(schedule, validLayout(DefaultTestId + i, DefaultTestPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultTestId + 1, DefaultTestId + 2, DefaultTestId + 3}));
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsSamePrioritySomeNotInCache)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    addToQueue(schedule, validLayout(DefaultTestId + 1, DefaultTestPriority));
    addToQueue(schedule, notInCacheLayout(DefaultTestId + 2, DefaultTestPriority));
    addToQueue(schedule, validLayout(DefaultTestId + 3, DefaultTestPriority));
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultTestId + 1, DefaultTestId + 3}));
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsSamePrioritySomeNotInRange)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    addToQueue(schedule, validLayout(DefaultTestId + 1, DefaultTestPriority));
    addToQueue(schedule, notInRangeLayout(DefaultTestId + 2, DefaultTestPriority));
    addToQueue(schedule, validLayout(DefaultTestId + 3, DefaultTestPriority));
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultTestId + 1, DefaultTestId + 3}));
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsDifferentPrioritiesAllValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, validLayout(DefaultTestId + i, DefaultTestPriority + i));
    }
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), std::vector{DefaultTestId + 3});
}

TEST_F(SchedulerLayoutTests, OverlayLayoutsDifferentPrioritiesSomeInvalid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    addToQueue(schedule, validLayout(DefaultTestId + 1, DefaultTestPriority + 1));
    addToQueue(schedule, notInRangeLayout(DefaultTestId + 2, DefaultTestPriority + 2));
    addToQueue(schedule, notInCacheLayout(DefaultTestId + 3, DefaultTestPriority + 3));
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->overlayLayouts(), std::vector{DefaultTestId + 1});
}
