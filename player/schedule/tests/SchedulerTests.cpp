#include "SchedulerTests.hpp"
#include "Common.hpp"
#include "testutils/Utils.hpp"

#include "Scheduler.hpp"

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
    schedule.generatedTime += DateTimeHours(1);
    scheduler->reloadSchedule(std::move(schedule));

    ASSERT_TRUE(slot.called(2));
}
