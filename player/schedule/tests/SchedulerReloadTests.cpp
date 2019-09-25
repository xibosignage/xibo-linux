#include "SchedulerReloadTests.hpp"
#include "Scheduler.hpp"

#include "testutils/Utils.hpp"

using namespace testing;

const std::string FakeSchedule = "Fake";

TEST_F(SchedulerTests, UpdateWithNoPreviousSchedule)
{
    auto scheduler = construct();
    FakeSlot<void(const LayoutSchedule&)> slot;

    scheduler->scheduleUpdated().connect(slot);
    scheduler->reloadSchedule(ScheduleTests::schedule());

    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, UpdateTwiceWithSameSchedule)
{
    auto scheduler = construct();
    FakeSlot<void(const LayoutSchedule&)> slot;

    scheduler->scheduleUpdated().connect(slot);
    scheduler->reloadSchedule(ScheduleTests::schedule());
    scheduler->reloadSchedule(ScheduleTests::schedule());

    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, UpdateWithNewSchedule)
{
    auto scheduler = construct();
    FakeSlot<void(const LayoutSchedule&)> slot;

    scheduler->scheduleUpdated().connect(slot);
    scheduler->reloadSchedule(ScheduleTests::schedule());

    auto schedule = ScheduleTests::schedule();
    schedule.generatedTime = schedule.generatedTime + DateTime::Hours(1);
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_TRUE(slot.called(2));
}

TEST_F(SchedulerTests, DefaultLayoutReloadSameQueue)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultId);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadQueue();

    EXPECT_EQ(scheduler->nextLayout(), DefaultId);
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, DefaultReloadQueueWithRegularLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultId);
    auto newSchedule = makeRegularSchedule(DefaultId, DefaultId + 1, DefaultId + 2);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 2);
    ASSERT_TRUE(slot.called(2));
}

TEST_F(SchedulerTests, DefaultReloadQueueWithSameDefault)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultId);
    auto newSchedule = makeRegularSchedule(DefaultId);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId);
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, DefaultReloadQueueWithAnotherDefault)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultId);
    auto newSchedule = makeRegularSchedule(DefaultId + 1);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
    ASSERT_TRUE(slot.called(2));
}

TEST_F(SchedulerTests, RegularReloadSameQueue)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultId, DefaultId + 1, DefaultId + 2);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadQueue();

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 2);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, RegularReloadQueueWithAllNewLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultId, DefaultId + 1, DefaultId + 2);
    auto newSchedule = makeRegularSchedule(DefaultId, DefaultId + 3, DefaultId + 4);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 4);
    ASSERT_TRUE(slot.called(2));
}

TEST_F(SchedulerTests, RegularReloadQueueWithSomeNewLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultId, DefaultId + 1, DefaultId + 2);
    auto newSchedule = makeRegularSchedule(DefaultId, DefaultId + 3, DefaultId + 1, DefaultId + 4);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 4);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultId + 1);
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, OverlayReloadQueueWithSameLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeOverlaySchedule(DefaultId + 1, DefaultId + 2);
    auto newSchedule = makeOverlaySchedule(DefaultId + 1, DefaultId + 2);
    scheduler->overlaysUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultId + 1, DefaultId + 2}));
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, OverlayReloadQueueWithNewLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeOverlaySchedule(DefaultId + 1, DefaultId + 2);
    auto newSchedule = makeOverlaySchedule(DefaultId + 1, DefaultId + 3);
    scheduler->overlaysUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultId + 1, DefaultId + 3}));
    ASSERT_TRUE(slot.called(2));
}
