#include <gtest/gtest.h>

#include "ScheduleSerializer.hpp"
#include "common/fs/FilePath.hpp"

const std::string XmlToParse = R"(<?xml version="1.0"?>
                               <schedule generated="2019-01-08 08:00:00" fitlerFrom="2019-07-26 07:00:00" fitlerTo="2019-07-28 07:00:00">
                                 <layout file="3" fromdt="1970-01-01 01:00:00" todt="2038-01-19 03:14:07" scheduleid="2" priority="1" syncEvent="0">
                                   <dependents>
                                     <file>local1.txt</file>
                                     <file>local2.txt</file>
                                   </dependents>
                                 </layout>
                                 <overlays>
                                   <overlay file="2" fromdt="1970-01-01 01:00:00" todt="2038-01-19 03:14:07" scheduleid="1" priority="0"/>
                                 </overlays>
                                 <default file="1">
                                   <dependents>
                                     <file>defaultLocal1.txt</file>
                                     <file>defaultLocal2.txt</file>
                                   </dependents>
                                 </default>
                                 <dependants>
                                   <file>global1.txt</file>
                                   <file>global2.txt</file>
                                 </dependants>
                               </schedule>)";

const std::string SchedulePath = "fakeSchedule.xml";
const std::string SchedulePathInvalid = "fakeScheduleInvalid.xml";

void assertOverlay(const ScheduledLayout& overlay)
{
    EXPECT_EQ(overlay.scheduleId, 1);
    EXPECT_EQ(overlay.id, 2);
    EXPECT_EQ(overlay.priority, 0);
    EXPECT_EQ(overlay.dependants.size(), 0);
}

void assertDefault(const DefaultScheduledLayout& defaultLayout)
{
    EXPECT_EQ(defaultLayout.id, 1);

    EXPECT_EQ(defaultLayout.dependants.size(), 2);
    EXPECT_EQ(defaultLayout.dependants[0], "defaultLocal1.txt");
    EXPECT_EQ(defaultLayout.dependants[1], "defaultLocal2.txt");
}

void assertRegular(const ScheduledLayout& regular)
{
    EXPECT_EQ(regular.scheduleId, 2);
    EXPECT_EQ(regular.id, 3);
    EXPECT_EQ(regular.priority, 1);

    EXPECT_EQ(regular.dependants.size(), 2);
    EXPECT_EQ(regular.dependants[0], "local1.txt");
    EXPECT_EQ(regular.dependants[1], "local2.txt");
}

void assertDependants(const std::vector<std::string>& dependants)
{
    EXPECT_EQ(dependants.size(), 2);
    EXPECT_EQ(dependants[0], "global1.txt");
    EXPECT_EQ(dependants[1], "global2.txt");
}

void assertSchedule(const LayoutSchedule& schedule)
{
    ASSERT_EQ(schedule.regularLayouts.size(), 1);
    ASSERT_EQ(schedule.overlayLayouts.size(), 1);
    EXPECT_EQ(schedule.generatedTime, "2019-01-08 08:00:00");

    assertOverlay(schedule.overlayLayouts.front());
    assertDefault(schedule.defaultLayout);
    assertRegular(schedule.regularLayouts.front());
    assertDependants(schedule.globalDependants);
}

TEST(ScheduleSerializer, LoadFromFile)
{
    ScheduleSerializer serializer;

    assertSchedule(serializer.parseSchedule(FilePath{SchedulePath}));
}

TEST(ScheduleSerializer, LoadFromFileInvalid)
{
    ScheduleSerializer serializer;

    ASSERT_THROW(serializer.parseSchedule(FilePath{SchedulePathInvalid}), ScheduleParseException);
}

TEST(ScheduleSerializer, LoadFromString)
{
    ScheduleSerializer serializer;

    assertSchedule(serializer.parseSchedule(XmlToParse));
}

TEST(ScheduleSerializer, LoadFromStringInvalid)
{
    ScheduleSerializer serializer;

    ASSERT_THROW(serializer.parseSchedule(std::string{"invalid"}), ScheduleParseException);
}
