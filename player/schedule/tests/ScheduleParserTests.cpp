#include <gtest/gtest.h>

#include "Common.hpp"
#include "ScheduleParser.hpp"
#include "common/fs/FilePath.hpp"

const std::string ScheduleReadPath = "fakeScheduleRead.xml";
const std::string ScheduleReadPathInvalid = "fakeScheduleInvalid.xml";

TEST(ScheduleParser, LoadFromExistingFile)
{
    ScheduleParser parser;

    ASSERT_EQ(ScheduleTests::schedule(), parser.scheduleFrom(FilePath{ScheduleReadPath}));
}

TEST(ScheduleParser, LoadFromNonExistingFile)
{
    ScheduleParser parser;

    ASSERT_EQ(LayoutSchedule{}, parser.scheduleFrom(FilePath{"non-existing"}));
}

TEST(ScheduleParser, LoadFromFileInvalid)
{
    ScheduleParser parser;

    ASSERT_THROW(parser.scheduleFrom(FilePath{ScheduleReadPathInvalid}), ScheduleParseException);
}

TEST(ScheduleParser, LoadFromString)
{
    ScheduleParser parser;

    ASSERT_EQ(ScheduleTests::schedule(), parser.scheduleFrom(ScheduleXml));
}

TEST(ScheduleParser, LoadFromStringInvalid)
{
    ScheduleParser parser;

    ASSERT_THROW(parser.scheduleFrom(std::string{"invalid"}), ScheduleParseException);
}
