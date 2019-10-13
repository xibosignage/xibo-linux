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
    ASSERT_FALSE(queue.inQueue(EmptyLayoutId));

    EXPECT_EQ(queue.next(), EmptyLayoutId);
    EXPECT_EQ(queue.current(), EmptyLayoutId);
}

TEST(RegularLayoutQueue, NextLayoutOnDefaultLayout)
{
    auto queue = queueWithPriorities<RegularLayoutQueue>({});
    queue.addDefault(DefaultScheduledLayout{DefaultId, {}});

    ASSERT_EQ(queue.size(), 0);
    ASSERT_TRUE(queue.inQueue(DefaultId));

    EXPECT_EQ(queue.next(), DefaultId);
    EXPECT_EQ(queue.current(), DefaultId);
}

TEST(RegularLayoutQueue, NextLayoutGivesTheHighestPriority)
{
    auto queue = queueWithPriorities<RegularLayoutQueue>({0, 1, 2});

    ASSERT_EQ(queue.size(), 1);
    ASSERT_FALSE(queue.inQueue(DefaultId));
    ASSERT_FALSE(queue.inQueue(DefaultId + 1));
    ASSERT_TRUE(queue.inQueue(DefaultId + 2));

    EXPECT_EQ(queue.next(), DefaultId + 2);
    EXPECT_EQ(queue.current(), DefaultId + 2);
    EXPECT_EQ(queue.next(), DefaultId + 2);
    EXPECT_EQ(queue.current(), DefaultId + 2);
}

TEST(RegularLayoutQueue, NextLayoutShouldPreservePosition)
{
    auto queue = queueWithSamePriorities<RegularLayoutQueue>();
    queue.addDefault(DefaultScheduledLayout{DefaultId + 3, {}});

    ASSERT_EQ(queue.size(), 3);
    ASSERT_TRUE(queue.inQueue(DefaultId));
    ASSERT_TRUE(queue.inQueue(DefaultId + 1));
    ASSERT_TRUE(queue.inQueue(DefaultId + 2));

    EXPECT_EQ(queue.next(), DefaultId);
    EXPECT_EQ(queue.current(), DefaultId);
    EXPECT_EQ(queue.next(), DefaultId + 1);
    EXPECT_EQ(queue.current(), DefaultId + 1);
    EXPECT_EQ(queue.next(), DefaultId + 2);
    EXPECT_EQ(queue.current(), DefaultId + 2);
    EXPECT_EQ(queue.next(), DefaultId);
    EXPECT_EQ(queue.current(), DefaultId);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutScheduledAndDefault)
{
    const size_t QueueSize = 5;
    auto queue = queueWithSamePriorities<RegularLayoutQueue>(QueueSize);
    queue.addDefault(DefaultScheduledLayout{DefaultId + 5, {}});

    EXPECT_EQ(queue.next(), DefaultId);
    EXPECT_EQ(queue.current(), DefaultId);

    queue.updateCurrent(DefaultId + 3);

    EXPECT_EQ(queue.next(), DefaultId + 4);
    EXPECT_EQ(queue.current(), DefaultId + 4);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutNotInQueueScheduled)
{
    const size_t QueueSize = 5;
    auto queue = queueWithSamePriorities<RegularLayoutQueue>(QueueSize);
    queue.addDefault(DefaultScheduledLayout{DefaultId + 5, {}});

    EXPECT_EQ(queue.next(), DefaultId);
    EXPECT_EQ(queue.current(), DefaultId);

    queue.updateCurrent(DefaultId + 10);

    EXPECT_EQ(queue.next(), DefaultId + 1);
    EXPECT_EQ(queue.current(), DefaultId + 1);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutWithDefaultInFilledQueue)
{
    const size_t QueueSize = 5;
    auto queue = queueWithSamePriorities<RegularLayoutQueue>(QueueSize);
    queue.addDefault(DefaultScheduledLayout{DefaultId + 5, {}});

    EXPECT_EQ(queue.next(), DefaultId);
    EXPECT_EQ(queue.current(), DefaultId);

    queue.updateCurrent(DefaultId + 5);

    EXPECT_EQ(queue.next(), DefaultId + 1);
    EXPECT_EQ(queue.current(), DefaultId + 1);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutWithDefaultOnly)
{
    auto queue = queueWithSamePriorities<RegularLayoutQueue>({});
    queue.addDefault(DefaultScheduledLayout{DefaultId, {}});

    EXPECT_EQ(queue.current(), EmptyLayoutId);

    queue.updateCurrent(DefaultId);

    EXPECT_EQ(queue.current(), DefaultId);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutNotInQueueDefault)
{
    auto queue = queueWithSamePriorities<RegularLayoutQueue>({});
    queue.addDefault(DefaultScheduledLayout{DefaultId, {}});

    EXPECT_EQ(queue.current(), EmptyLayoutId);

    queue.updateCurrent(DefaultId + 5);

    EXPECT_EQ(queue.current(), EmptyLayoutId);
}
