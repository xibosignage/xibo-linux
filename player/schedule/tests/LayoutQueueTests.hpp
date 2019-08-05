#pragma once

#include <gtest/gtest.h>

#include "RegularLayoutQueue.hpp"
#include "OverlayLayoutQueue.hpp"
#include "Common.hpp"

const std::vector<std::vector<int>> Priorities = {{0, 1, 2}, {0, 2, 1},
                                                  {1, 2, 0}, {1, 0, 2},
                                                  {2, 1, 0}, {2, 0, 1}};

template<typename Queue>
Queue queueWithDifferentPriorities(const std::vector<int>& priorities)
{
    Queue queue;

    for(size_t i = 0; i != priorities.size(); ++i)
    {
        int step = static_cast<int>(i);
        queue.add(ScheduleTests::scheduledLayout(DefaultSchediledId + step, DefaultId + step, priorities[i]));
    }

    return queue;
}

template<typename Queue>
Queue queueWithSamePriorities(size_t queueSize = 3)
{
    std::vector<int> priorities(queueSize, DefaultPriority);

    return queueWithDifferentPriorities<Queue>(priorities);
}

template<typename T>
struct PriorityQueueTest : public testing::Test
{
    using QueueType = T;
};

struct PriorityQueueTestPriorities : public testing::TestWithParam<std::vector<int>>
{
};
