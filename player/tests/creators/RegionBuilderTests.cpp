#include "RegionBuilderTests.hpp"

using namespace testing;

TEST(RegionBuilderSizeTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    auto adaptor = std::make_unique<testing::NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*adaptor, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    RegionBuilderTest().adaptor(std::move(adaptor)).defaultContent().defaultSize().build();
}

TEST_P(RegionBuilderSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    ASSERT_THROW(RegionBuilderTest().defaultContent().width(GetParam().width).height(GetParam().height).build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, RegionBuilderSizeTest, ::testing::ValuesIn(invalidRegionSizes));

TEST(RegionBuilderTest, Construct_WithContent_HandlerAddContentWithPosShouldBeCalled)
{
    auto adaptor = std::make_unique<testing::NiceMock<MockFixedLayoutAdaptor>>();

    EXPECT_CALL(*adaptor, addChild(_, _, _));

    RegionBuilderTest().adaptor(std::move(adaptor)).defaultContent().defaultSize().build();
}

TEST(RegionBuilderTest, Construct_CustomZorder_RegionWithCustomZorderCreated)
{
    auto region = RegionBuilderTest().defaultContent().defaultSize().zorder(DEFAULT_ZORDER + 1).build();

    ASSERT_EQ(region->zorder(), DEFAULT_ZORDER + 1);
}

TEST(RegionBuilderTest, Construct_CustomId_RegionWithCustomIdCreated)
{
    auto region = RegionBuilderTest().defaultContent().defaultSize().id(DEFAULT_ID + 1).build();

    ASSERT_EQ(region->id(), DEFAULT_ID + 1);
}

TEST(RegionBuilderTest, Construct_LoopedTrue_RegionWithLoopedContentCreated)
{
    auto region = RegionBuilderTest().defaultContent().defaultSize().loop(true).build();

    ASSERT_EQ(region->contentLooped(), true);
}

TEST(RegionBuilderTest, Construct_LoopedFalse_RegionWithoutLoopedContentCreated)
{
    auto region = RegionBuilderTest().defaultContent().defaultSize().loop(false).build();

    ASSERT_EQ(region->contentLooped(), false);
}
