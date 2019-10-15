#include "schedule/tests/SchedulerReloadTests.hpp"
#include "schedule/Scheduler.hpp"

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

    auto schedule = makeRegularSchedule(DefaultTestId);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadQueue();

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId);
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, DefaultReloadQueueWithRegularLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultTestId);
    auto newSchedule = makeRegularSchedule(DefaultTestId, DefaultTestId + 1, DefaultTestId + 2);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 2);
    ASSERT_TRUE(slot.called(2));
}

TEST_F(SchedulerTests, DefaultReloadQueueWithSameDefault)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultTestId);
    auto newSchedule = makeRegularSchedule(DefaultTestId);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId);
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, DefaultReloadQueueWithAnotherDefault)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultTestId);
    auto newSchedule = makeRegularSchedule(DefaultTestId + 1);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
    ASSERT_TRUE(slot.called(2));
}

TEST_F(SchedulerTests, RegularReloadSameQueue)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultTestId, DefaultTestId + 1, DefaultTestId + 2);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadQueue();

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 2);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, RegularReloadQueueWithAllNewLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultTestId, DefaultTestId + 1, DefaultTestId + 2);
    auto newSchedule = makeRegularSchedule(DefaultTestId, DefaultTestId + 3, DefaultTestId + 4);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 4);
    ASSERT_TRUE(slot.called(2));
}

TEST_F(SchedulerTests, RegularReloadQueueWithSomeNewLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeRegularSchedule(DefaultTestId, DefaultTestId + 1, DefaultTestId + 2);
    auto newSchedule = makeRegularSchedule(DefaultTestId, DefaultTestId + 3, DefaultTestId + 1, DefaultTestId + 4);
    scheduler->layoutUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->nextLayout();  // trigger layout playing
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 4);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 3);
    EXPECT_EQ(scheduler->nextLayout(), DefaultTestId + 1);
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, OverlayReloadQueueWithSameLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeOverlaySchedule(DefaultTestId + 1, DefaultTestId + 2);
    auto newSchedule = makeOverlaySchedule(DefaultTestId + 1, DefaultTestId + 2);
    scheduler->overlaysUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultTestId + 1, DefaultTestId + 2}));
    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(SchedulerTests, OverlayReloadQueueWithNewLayouts)
{
    auto scheduler = construct();
    FakeSlot<void()> slot;

    auto schedule = makeOverlaySchedule(DefaultTestId + 1, DefaultTestId + 2);
    auto newSchedule = makeOverlaySchedule(DefaultTestId + 1, DefaultTestId + 3);
    scheduler->overlaysUpdated().connect(slot);
    scheduler->reloadSchedule(std::move(schedule));
    scheduler->reloadSchedule(std::move(newSchedule));

    EXPECT_EQ(scheduler->overlayLayouts(), (std::vector{DefaultTestId + 1, DefaultTestId + 3}));
    ASSERT_TRUE(slot.called(2));
}
