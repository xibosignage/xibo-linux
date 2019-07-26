#include "LayoutListTests.hpp"

using ListsTypes = ::testing::Types<RegularLayoutList, OverlayLayoutList>;
TYPED_TEST_CASE(PriorityListTest, ListsTypes);

TYPED_TEST(PriorityListTest, AddLayoutsWithSamePriority)
{
    auto list = listWithSamePriorities<typename TestFixture::ListType>();

    ASSERT_EQ(list.size(), 3);
}

TEST_P(PriorityListTestPriorities, AddLayoutsWithDifferentPriorities)
{
    auto overlayList = listWithDifferentPriorities<OverlayLayoutList>(GetParam());

    ASSERT_EQ(overlayList.size(), 1);
    ASSERT_EQ(overlayList.begin()->priority, 2);
}

INSTANTIATE_TEST_CASE_P(Suite, PriorityListTestPriorities, ::testing::ValuesIn(Priorities));
