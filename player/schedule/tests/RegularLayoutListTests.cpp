#include "LayoutContainerTests.hpp"

TEST(RegularLayoutList, NextLayoutShouldPreservePosition)
{
    auto list = listWithSamePriorities<RegularLayoutList>();
    ScheduledLayout next;

    ASSERT_EQ(list.size(), 3);

    next = list.nextLayout();
    EXPECT_EQ(next.id, DefaultId);
    next = list.nextLayout();
    EXPECT_EQ(next.id, DefaultId + 1);
    next = list.nextLayout();
    EXPECT_EQ(next.id, DefaultId + 2);
    next = list.nextLayout();
    EXPECT_EQ(next.id, DefaultId);
}

TEST(RegularLayoutList, NextLayoutWorksOnlyOnTheHighestPriorities)
{
    auto list = listWithDifferentPriorities<RegularLayoutList>({0, 1, 2});
    ScheduledLayout next;

    ASSERT_EQ(list.size(), 1);

    next = list.nextLayout();
    EXPECT_EQ(next.id, DefaultId + 2);
    next = list.nextLayout();
    EXPECT_EQ(next.id, DefaultId + 2);
}
