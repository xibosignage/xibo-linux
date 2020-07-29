#pragma once

#include <gtest/gtest.h>

#include "schedule/OverlayLayoutQueue.hpp"
#include "schedule/RegularLayoutQueue.hpp"
#include "schedule/tests/Common.hpp"

const std::vector<std::vector<int>> PrioritiesToTest =
    {{0, 1, 2}, {0, 2, 1}, {1, 2, 0}, {1, 0, 2}, {2, 1, 0}, {2, 0, 1}};

template <typename Queue>
Queue queueWithPriorities(const std::vector<int>& priorities)
{
    Queue queue;

    for (size_t i = 0; i != priorities.size(); ++i)
    {
        int step = static_cast<int>(i);
        queue.add(ScheduleTests::scheduledLayout(DefaultScheduleId + step, DefaultTestId + step, priorities[i]));
    }

    return queue;
}

template <typename Queue>
Queue queueWithSamePriorities(size_t queueSize = 3)
{
    std::vector<int> priorities(queueSize, DefaultTestPriority);

    return queueWithPriorities<Queue>(priorities);
}

template <typename T>
struct LayoutQueueTest : public testing::Test
{
    using QueueType = T;
};
