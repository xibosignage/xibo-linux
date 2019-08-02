#pragma once

#include <gtest/gtest.h>

#include "LayoutContainer.hpp"
#include "Common.hpp"

const std::vector<std::vector<int>> Priorities = {{0, 1, 2}, {0, 2, 1},
                                                  {1, 2, 0}, {1, 0, 2},
                                                  {2, 1, 0}, {2, 0, 1}};

template<typename List>
List listWithDifferentPriorities(const std::vector<int>& priorities)
{
    List list;

    for(size_t i = 0; i != priorities.size(); ++i)
    {
        int step = static_cast<int>(i);
        list.add(ScheduleTests::scheduledLayout(DefaultSchediledId + step, DefaultId + step, priorities[i]));
    }

    return list;
}

template<typename List>
List listWithSamePriorities(size_t listSize = 3)
{
    std::vector<int> priorities(listSize, DefaultPriority);

    return listWithDifferentPriorities<List>(priorities);
}

template<typename T>
struct PriorityListTest : public testing::Test
{
    using ListType = T;
};

struct PriorityListTestPriorities : public testing::TestWithParam<std::vector<int>>
{
};
