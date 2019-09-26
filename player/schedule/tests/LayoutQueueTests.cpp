#include "LayoutQueueTests.hpp"

using QueueTypes = ::testing::Types<RegularLayoutQueue, OverlayLayoutQueue>;
TYPED_TEST_CASE(LayoutQueueTest, QueueTypes);

TYPED_TEST(LayoutQueueTest, LayoutsWithSamePriority)
{
    auto queue = queueWithSamePriorities<typename TestFixture::QueueType>();

    ASSERT_EQ(queue.size(), 3);
}

TYPED_TEST(LayoutQueueTest, ClearAndEmpty)
{
    auto queue = queueWithSamePriorities<typename TestFixture::QueueType>();

    queue.clear();

    ASSERT_EQ(queue.size(), 0);
    ASSERT_TRUE(queue.empty());
}

TYPED_TEST(LayoutQueueTest, LayoutsWithDifferentPriorities)
{
    for (auto&& priorities : PrioritiesToTest)
    {
        auto queue = queueWithPriorities<typename TestFixture::QueueType>(priorities);

        EXPECT_EQ(queue.size(), 1);
        EXPECT_EQ(queue.begin()->priority, 2);
    }
}

TEST(RegularLayoutQueue, NextLayoutOnEmptyQueue)
{
    auto queue = queueWithPriorities<RegularLayoutQueue>({});

    ASSERT_EQ(queue.size(), 0);

    EXPECT_EQ(queue.next(), EmptyLayoutId);
}

TEST(RegularLayoutQueue, NextLayoutOnDefaultLayout)
{
    auto queue = queueWithPriorities<RegularLayoutQueue>({});
    queue.addDefault(DefaultScheduledLayout{DefaultId, {}});

    ASSERT_EQ(queue.size(), 0);

    EXPECT_EQ(queue.next(), DefaultId);
}

TEST(RegularLayoutQueue, NextLayoutGivesTheHighestPriority)
{
    auto queue = queueWithPriorities<RegularLayoutQueue>({0, 1, 2});

    ASSERT_EQ(queue.size(), 1);

    EXPECT_EQ(queue.next(), DefaultId + 2);
    EXPECT_EQ(queue.next(), DefaultId + 2);
}

TEST(RegularLayoutQueue, NextLayoutShouldPreservePosition)
{
    auto queue = queueWithSamePriorities<RegularLayoutQueue>();
    queue.addDefault(DefaultScheduledLayout{DefaultId + 3, {}});

    ASSERT_EQ(queue.size(), 3);

    EXPECT_EQ(queue.next(), DefaultId);
    EXPECT_EQ(queue.next(), DefaultId + 1);
    EXPECT_EQ(queue.next(), DefaultId + 2);
    EXPECT_EQ(queue.next(), DefaultId);
}
