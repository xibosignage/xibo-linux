#include "schedule/Scheduler.hpp"
#include "schedule/tests/SchedulerLayoutTests.hpp"

static void addToQueue(LayoutSchedule& schedule, ScheduledLayout&& type)
{
    schedule.regularLayouts.emplace_back(std::move(type));
}

TEST_F(SchedulerLayoutTests, NoRegularLayoutsDefaultLayoutInvalid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout.id = DefaultTestId;
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->nextLayout(), EmptyLayoutId);
}

TEST_F(SchedulerLayoutTests, NoRegularLayoutsDefaultLayoutValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultTestId);
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->nextLayout(), DefaultTestId);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePriorityAllNotInCache)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultTestId);
    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, notInCacheLayout(DefaultTestId + i, DefaultTestPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePriorityAllNotInRange)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultTestId);
    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, notInRangeLayout(DefaultTestId + i, DefaultTestPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePriorityAllValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultTestId);
    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, validLayout(DefaultTestId + i, DefaultTestPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 2);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePrioritySomeNotInCache)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultTestId);
    addToQueue(schedule, validLayout(DefaultTestId + 1, DefaultTestPriority));
    addToQueue(schedule, notInCacheLayout(DefaultTestId + 2, DefaultTestPriority));
    addToQueue(schedule, validLayout(DefaultTestId + 3, DefaultTestPriority));
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePrioritySomeNotInRange)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultTestId);
    addToQueue(schedule, validLayout(DefaultTestId + 1, DefaultTestPriority));
    addToQueue(schedule, notInRangeLayout(DefaultTestId + 2, DefaultTestPriority));
    addToQueue(schedule, validLayout(DefaultTestId + 3, DefaultTestPriority));
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsDifferentPrioritiesAllValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultTestId);
    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, validLayout(DefaultTestId + i, DefaultTestPriority + i));
    }
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 3);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsDifferentPrioritiesSomeInvalid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultTestId);
    addToQueue(schedule, validLayout(DefaultTestId + 1, DefaultTestPriority + 1));
    addToQueue(schedule, notInRangeLayout(DefaultTestId + 2, DefaultTestPriority + 2));
    addToQueue(schedule, notInCacheLayout(DefaultTestId + 3, DefaultTestPriority + 3));
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
}
