#include <gtest/gtest.h>

#include "common/fs/FilePath.hpp"
#include "schedule/ScheduleParser.hpp"
#include "schedule/tests/Common.hpp"

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

    ASSERT_THROW(parser.scheduleFrom(FilePath{ScheduleReadPathInvalid}), ScheduleParser::Error);
}

TEST(ScheduleParser, LoadFromString)
{
    ScheduleParser parser;

    ASSERT_EQ(ScheduleTests::schedule(), parser.scheduleFrom(ScheduleXml));
}

TEST(ScheduleParser, LoadFromStringInvalid)
{
    ScheduleParser parser;

    ASSERT_THROW(parser.scheduleFrom(std::string{"invalid"}), ScheduleParser::Error);
}
