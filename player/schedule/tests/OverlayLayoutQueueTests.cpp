#include "LayoutQueueTests.hpp"

using QueueTypes = ::testing::Types<RegularLayoutQueue, OverlayLayoutQueue>;
TYPED_TEST_CASE(PriorityQueueTest, QueueTypes);

TYPED_TEST(PriorityQueueTest, AddLayoutsWithSamePriority)
{
    auto list = queueWithSamePriorities<typename TestFixture::QueueType>();

    ASSERT_EQ(list.size(), 3);
}

TEST_P(PriorityQueueTestPriorities, AddLayoutsWithDifferentPriorities)
{
    auto overlayList = queueWithDifferentPriorities<OverlayLayoutQueue>(GetParam());

    ASSERT_EQ(overlayList.size(), 1);
    ASSERT_EQ(overlayList.begin()->priority, 2);
}

INSTANTIATE_TEST_CASE_P(Suite, PriorityQueueTestPriorities, ::testing::ValuesIn(Priorities));
