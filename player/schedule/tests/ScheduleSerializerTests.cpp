#include <gtest/gtest.h>

#include "ScheduleSerializer.hpp"
#include "Common.hpp"

#include "common/fs/FilePath.hpp"
#include "common/DateTimeProvider.hpp"

const std::string SchedulePath = "fakeSchedule.xml";
const std::string SchedulePathInvalid = "fakeScheduleInvalid.xml";

TEST(ScheduleSerializer, LoadFromFile)
{
    ScheduleSerializer serializer;

    ASSERT_EQ(ScheduleTests::layoutSchedule(), serializer.parseSchedule(FilePath{SchedulePath}));
}

TEST(ScheduleSerializer, LoadFromFileInvalid)
{
    ScheduleSerializer serializer;

    ASSERT_THROW(serializer.parseSchedule(FilePath{SchedulePathInvalid}), ScheduleParseException);
}

TEST(ScheduleSerializer, LoadFromString)
{
    ScheduleSerializer serializer;

    ASSERT_EQ(ScheduleTests::layoutSchedule(), serializer.parseSchedule(ScheduleXml));
}

TEST(ScheduleSerializer, LoadFromStringInvalid)
{
    ScheduleSerializer serializer;

    ASSERT_THROW(serializer.parseSchedule(std::string{"invalid"}), ScheduleParseException);
}
