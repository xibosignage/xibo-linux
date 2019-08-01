#include "ScheduleManagerTests.hpp"
#include "Common.hpp"
#include "testutils/Utils.hpp"

#include "ScheduleManager.hpp"

using namespace testing;

const std::string FakeSchedule = "Fake";

TEST_F(ScheduleManagerTests, LoadExistingFile)
{
    auto manager = construct();

    ON_CALL(*fs, exists(_)).WillByDefault(Return(true));
    EXPECT_CALL(*serializer, parseSchedule(A<const FilePath&>())).Times(1);

    manager->load(FilePath{});
}

TEST_F(ScheduleManagerTests, LoadNonExistingFile)
{
    auto manager = construct();

    ON_CALL(*fs, exists(_)).WillByDefault(Return(false));
    EXPECT_CALL(*serializer, parseSchedule(A<const FilePath&>())).Times(0);

    manager->load(FilePath{});
}

TEST_F(ScheduleManagerTests, UpdateWithNoPreviousSchedule)
{
    auto manager = construct();
    FakeSlot<void(const LayoutSchedule&)> slot;

    ON_CALL(*serializer, parseSchedule(A<const std::string&>())).WillByDefault(Return(ScheduleTests::layoutSchedule()));
    manager->scheduleUpdated().connect(slot);
    manager->update(FakeSchedule);

    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(ScheduleManagerTests, UpdateTwiceWithSameSchedule)
{
    auto manager = construct();
    FakeSlot<void(const LayoutSchedule&)> slot;

    ON_CALL(*serializer, parseSchedule(A<const std::string&>())).WillByDefault(Return(ScheduleTests::layoutSchedule()));
    manager->scheduleUpdated().connect(slot);
    manager->update(FakeSchedule);
    manager->update(FakeSchedule);

    ASSERT_TRUE(slot.calledOnce());
}

TEST_F(ScheduleManagerTests, UpdateWithNewSchedule)
{
//    auto manager = construct();

//    EXPECT_CALL(*fs, writeToFile(_, _)).Times(1);

//    manager->update(FakeSchedule);
//    manager->update(ScheduleXml);
}
