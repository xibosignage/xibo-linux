#pragma once

#include <gtest/gtest.h>

#include "Common.hpp"
#include "OverlayLayoutQueue.hpp"
#include "RegularLayoutQueue.hpp"

const std::vector<std::vector<int>> PrioritiesToTest = {{0, 1, 2}, {0, 2, 1}, {1, 2, 0},
                                                        {1, 0, 2}, {2, 1, 0}, {2, 0, 1}};

template <typename Queue>
Queue queueWithPriorities(const std::vector<LayoutId>& priorities)
{
    Queue queue;

    for (size_t i = 0; i != priorities.size(); ++i)
    {
        int step = static_cast<int>(i);
        queue.add(ScheduleTests::scheduledLayout(DefaultSchediledId + step, DefaultId + step, priorities[i]));
    }

    return queue;
}

template <typename Queue>
Queue queueWithSamePriorities(size_t queueSize = 3)
{
    std::vector<LayoutId> priorities(queueSize, DefaultPriority);

    return queueWithPriorities<Queue>(priorities);
}

template <typename T>
struct LayoutQueueTest : public testing::Test
{
    using QueueType = T;
};
