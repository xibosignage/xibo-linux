#include "LayoutSchedulerTests.hpp"
#include "Common.hpp"
#include "testutils/Utils.hpp"

#include "LayoutScheduler.hpp"

using namespace testing;

const std::string FakeSchedule = "Fake";

TEST_F(LayoutSchedulerTests, UpdateWithNoPreviousSchedule)
{
    auto scheduler = construct();
    FakeSlot<void(const LayoutSchedule&)> slot;

    scheduler->scheduleUpdated().connect(slot);
    scheduler->reloadSchedule(ScheduleTests::layoutSchedule());

    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(LayoutSchedulerTests, UpdateTwiceWithSameSchedule)
{
    auto scheduler = construct();
    FakeSlot<void(const LayoutSchedule&)> slot;

    scheduler->scheduleUpdated().connect(slot);
    scheduler->reloadSchedule(ScheduleTests::layoutSchedule());
    scheduler->reloadSchedule(ScheduleTests::layoutSchedule());

    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(LayoutSchedulerTests, UpdateWithNewSchedule)
{
//    auto manager = construct();

//    EXPECT_CALL(*fs, writeToFile(_, _)).Times(1);

//    manager->update(FakeSchedule);
//    manager->update(ScheduleXml);
}
