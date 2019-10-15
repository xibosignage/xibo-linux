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
    queue.addDefault(DefaultScheduledLayout{DefaultTestId, {}});

    ASSERT_EQ(queue.size(), 0);
    ASSERT_TRUE(queue.inQueue(DefaultTestId));

    EXPECT_EQ(queue.next(), DefaultTestId);
    EXPECT_EQ(queue.current(), DefaultTestId);
}

TEST(RegularLayoutQueue, NextLayoutGivesTheHighestPriority)
{
    auto queue = queueWithPriorities<RegularLayoutQueue>({0, 1, 2});

    ASSERT_EQ(queue.size(), 1);
    ASSERT_FALSE(queue.inQueue(DefaultTestId));
    ASSERT_FALSE(queue.inQueue(DefaultTestId + 1));
    ASSERT_TRUE(queue.inQueue(DefaultTestId + 2));

    EXPECT_EQ(queue.next(), DefaultTestId + 2);
    EXPECT_EQ(queue.current(), DefaultTestId + 2);
    EXPECT_EQ(queue.next(), DefaultTestId + 2);
    EXPECT_EQ(queue.current(), DefaultTestId + 2);
}

TEST(RegularLayoutQueue, NextLayoutShouldPreservePosition)
{
    auto queue = queueWithSamePriorities<RegularLayoutQueue>();
    queue.addDefault(DefaultScheduledLayout{DefaultTestId + 3, {}});

    ASSERT_EQ(queue.size(), 3);
    ASSERT_TRUE(queue.inQueue(DefaultTestId));
    ASSERT_TRUE(queue.inQueue(DefaultTestId + 1));
    ASSERT_TRUE(queue.inQueue(DefaultTestId + 2));

    EXPECT_EQ(queue.next(), DefaultTestId);
    EXPECT_EQ(queue.current(), DefaultTestId);
    EXPECT_EQ(queue.next(), DefaultTestId + 1);
    EXPECT_EQ(queue.current(), DefaultTestId + 1);
    EXPECT_EQ(queue.next(), DefaultTestId + 2);
    EXPECT_EQ(queue.current(), DefaultTestId + 2);
    EXPECT_EQ(queue.next(), DefaultTestId);
    EXPECT_EQ(queue.current(), DefaultTestId);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutScheduledAndDefault)
{
    const size_t QueueSize = 5;
    auto queue = queueWithSamePriorities<RegularLayoutQueue>(QueueSize);
    queue.addDefault(DefaultScheduledLayout{DefaultTestId + 5, {}});

    EXPECT_EQ(queue.next(), DefaultTestId);
    EXPECT_EQ(queue.current(), DefaultTestId);

    queue.updateCurrent(DefaultTestId + 3);

    EXPECT_EQ(queue.next(), DefaultTestId + 4);
    EXPECT_EQ(queue.current(), DefaultTestId + 4);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutNotInQueueScheduled)
{
    const size_t QueueSize = 5;
    auto queue = queueWithSamePriorities<RegularLayoutQueue>(QueueSize);
    queue.addDefault(DefaultScheduledLayout{DefaultTestId + 5, {}});

    EXPECT_EQ(queue.next(), DefaultTestId);
    EXPECT_EQ(queue.current(), DefaultTestId);

    queue.updateCurrent(DefaultTestId + 10);

    EXPECT_EQ(queue.next(), DefaultTestId + 1);
    EXPECT_EQ(queue.current(), DefaultTestId + 1);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutWithDefaultInFilledQueue)
{
    const size_t QueueSize = 5;
    auto queue = queueWithSamePriorities<RegularLayoutQueue>(QueueSize);
    queue.addDefault(DefaultScheduledLayout{DefaultTestId + 5, {}});

    EXPECT_EQ(queue.next(), DefaultTestId);
    EXPECT_EQ(queue.current(), DefaultTestId);

    queue.updateCurrent(DefaultTestId + 5);

    EXPECT_EQ(queue.next(), DefaultTestId + 1);
    EXPECT_EQ(queue.current(), DefaultTestId + 1);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutWithDefaultOnly)
{
    auto queue = queueWithSamePriorities<RegularLayoutQueue>({});
    queue.addDefault(DefaultScheduledLayout{DefaultTestId, {}});

    EXPECT_EQ(queue.current(), EmptyLayoutId);

    queue.updateCurrent(DefaultTestId);

    EXPECT_EQ(queue.current(), DefaultTestId);
}

TEST(RegularLayoutQueue, UpdateCurrentLayoutNotInQueueDefault)
{
    auto queue = queueWithSamePriorities<RegularLayoutQueue>({});
    queue.addDefault(DefaultScheduledLayout{DefaultTestId, {}});

    EXPECT_EQ(queue.current(), EmptyLayoutId);

    queue.updateCurrent(DefaultTestId + 5);

    EXPECT_EQ(queue.current(), EmptyLayoutId);
}
