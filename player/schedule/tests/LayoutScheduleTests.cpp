#include <gtest/gtest.h>

#include "schedule/LayoutSchedule.hpp"
#include "schedule/tests/Common.hpp"

TEST(LayoutScheduleEquality, Empty)
{
    LayoutSchedule first{}, second{};

    ASSERT_EQ(first, second);
}

TEST(LayoutScheduleEquality, ByGeneratedTime)
{
    LayoutSchedule first{}, second{};

    first.generatedTime = DateTime(DateTime::Date(2019, 1, 1), DateTime::Time(1, 1, 1));
    second.generatedTime = DateTime(DateTime::Date(2019, 1, 1), DateTime::Time(1, 2, 1));

    ASSERT_NE(first, second);
}

TEST(LayoutScheduleEquality, ByDefaultLayoutId)
{
    LayoutSchedule first{}, second{};

    first.defaultLayout.id = DefaultTestId;
    second.defaultLayout.id = DefaultTestId + 1;

    ASSERT_NE(first, second);
}

TEST(LayoutScheduleEquality, ByDefaultLayoutDependantsSize)
{
    LayoutSchedule first{}, second{};

    first.defaultLayout.dependants.push_back({});

    ASSERT_NE(first, second);
}

TEST(LayoutScheduleEquality, ByDefaultLayoutDependantsContent)
{
    LayoutSchedule first{}, second{};

    first.defaultLayout.dependants.push_back("default1.txt");
    second.defaultLayout.dependants.push_back("default2.txt");

    ASSERT_NE(first, second);
}

TEST(LayoutScheduleEquality, ByRegularLayoutsSize)
{
    LayoutSchedule first{}, second{};

    first.regularLayouts.push_back({});

    ASSERT_NE(first, second);
}

TEST(LayoutScheduleEquality, ByOverlaysSize)
{
    LayoutSchedule first{}, second{};

    first.overlayLayouts.push_back({});

    ASSERT_NE(first, second);
}

TEST(LayoutScheduleEquality, ByGlobalDependantsSize)
{
    LayoutSchedule first{}, second{};

    first.globalDependants.push_back({});

    ASSERT_NE(first, second);
}

TEST(LayoutScheduleEquality, ByGlobalDependantsContent)
{
    LayoutSchedule first{}, second{};

    first.globalDependants.push_back("global1.txt");
    second.globalDependants.push_back("global2.txt");

    ASSERT_NE(first, second);
}

TEST(ScheduleLayoutEquality, Empty)
{
    ScheduledLayout first{}, second{};

    ASSERT_EQ(first, second);
}

TEST(ScheduleLayoutEquality, ById)
{
    ScheduledLayout first{}, second{};

    first.id = DefaultTestId;
    second.id = DefaultTestId + 1;

    ASSERT_NE(first, second);
}

TEST(ScheduleLayoutEquality, ByScheduleId)
{
    ScheduledLayout first{}, second{};

    first.scheduleId = DefaultScheduleId;
    second.scheduleId = DefaultScheduleId + 1;

    ASSERT_NE(first, second);
}

TEST(ScheduleLayoutEquality, ByPriority)
{
    ScheduledLayout first{}, second{};

    first.priority = DefaultTestPriority;
    second.priority = DefaultTestPriority + 1;

    ASSERT_NE(first, second);
}

TEST(ScheduleLayoutEquality, ByStartDt)
{
    ScheduledLayout first{}, second{};

    first.startDT = DateTime(DateTime::Date(2019, 1, 1), DateTime::Time(1, 1, 1));
    second.startDT = DateTime(DateTime::Date(2019, 1, 1), DateTime::Time(1, 2, 1));

    ASSERT_NE(first, second);
}

TEST(ScheduleLayoutEquality, ByEndDt)
{
    ScheduledLayout first{}, second{};

    first.endDT = DateTime(DateTime::Date(2019, 1, 1), DateTime::Time(1, 1, 1));
    second.endDT = DateTime(DateTime::Date(2019, 1, 1), DateTime::Time(1, 2, 1));

    ASSERT_NE(first, second);
}

TEST(ScheduleLayoutEquality, ByDependantsSize)
{
    ScheduledLayout first{}, second{};

    first.dependants.push_back({});

    ASSERT_NE(first, second);
}

TEST(ScheduleLayoutEquality, ByDependantsContent)
{
    ScheduledLayout first{}, second{};

    first.dependants.push_back("local1.txt");
    second.dependants.push_back("local2.txt");

    ASSERT_NE(first, second);
}
