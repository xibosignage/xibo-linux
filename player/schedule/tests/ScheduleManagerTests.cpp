#include "ScheduleManagerTests.hpp"
#include "ScheduleManager.hpp"

using namespace testing;

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
