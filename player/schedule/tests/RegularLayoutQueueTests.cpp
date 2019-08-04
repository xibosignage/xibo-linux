#include "LayoutQueueTests.hpp"

TEST(RegularLayoutQueue, NextLayoutShouldPreservePosition)
{
    auto queue = queueWithSamePriorities<RegularLayoutQueue>();
    ScheduledLayout next;

    ASSERT_EQ(queue.size(), 3);

    EXPECT_EQ(queue.next(), DefaultId);
    EXPECT_EQ(queue.next(), DefaultId + 1);
    EXPECT_EQ(queue.next(), DefaultId + 2);
    EXPECT_EQ(queue.next(), DefaultId);
}

TEST(RegularLayoutQueue, NextLayoutWorksOnlyOnTheHighestPriorities)
{
    auto queue = queueWithDifferentPriorities<RegularLayoutQueue>({0, 1, 2});
    ScheduledLayout next;

    ASSERT_EQ(queue.size(), 1);

    EXPECT_EQ(queue.next(), DefaultId + 2);
    EXPECT_EQ(queue.next(), DefaultId + 2);
}
