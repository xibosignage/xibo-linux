#include "Scheduler.hpp"
#include "SchedulerLayoutTests.hpp"

static void addToQueue(LayoutSchedule& schedule, ScheduledLayout&& type)
{
    schedule.regularLayouts.emplace_back(std::move(type));
}

TEST_F(SchedulerLayoutTests, NoRegularLayoutsDefaultLayoutInvalid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout.id = DefaultId;
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->nextLayout(), EmptyLayoutId);
}

TEST_F(SchedulerLayoutTests, NoRegularLayoutsDefaultLayoutValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultId);
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_EQ(scheduler->nextLayout(), DefaultId);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePriorityAllNotInCache)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultId);
    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, notInCacheLayout(DefaultId + i, DefaultPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePriorityAllNotInRange)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultId);
    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, notInRangeLayout(DefaultId + i, DefaultPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePriorityAllValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultId);
    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, validLayout(DefaultId + i, DefaultPriority));
    }
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 2);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePrioritySomeNotInCache)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultId);
    addToQueue(schedule, validLayout(DefaultId + 1, DefaultPriority));
    addToQueue(schedule, notInCacheLayout(DefaultId + 2, DefaultPriority));
    addToQueue(schedule, validLayout(DefaultId + 3, DefaultPriority));
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsSamePrioritySomeNotInRange)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultId);
    addToQueue(schedule, validLayout(DefaultId + 1, DefaultPriority));
    addToQueue(schedule, notInRangeLayout(DefaultId + 2, DefaultPriority));
    addToQueue(schedule, validLayout(DefaultId + 3, DefaultPriority));
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsDifferentPrioritiesAllValid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultId);
    for (int i = 1; i <= 3; ++i)
    {
        addToQueue(schedule, validLayout(DefaultId + i, DefaultPriority + i));
    }
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 3);
}

TEST_F(SchedulerLayoutTests, RegularLayoutsDifferentPrioritiesSomeInvalid)
{
    auto scheduler = construct();
    LayoutSchedule schedule{};

    schedule.defaultLayout = defaultLayout(DefaultId);
    addToQueue(schedule, validLayout(DefaultId + 1, DefaultPriority + 1));
    addToQueue(schedule, notInRangeLayout(DefaultId + 2, DefaultPriority + 2));
    addToQueue(schedule, notInCacheLayout(DefaultId + 3, DefaultPriority + 3));
    scheduler->reloadSchedule(std::move(schedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
}
