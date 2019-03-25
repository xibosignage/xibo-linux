#include "RegionTests.hpp"

using namespace ::testing;

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

const auto invalidRegionSizes = invalidSizes<MIN_WIDTH, MIN_HEIGHT>;

TEST_P(RegionConstructSizeTest, Construct_InvalidSize_ShouldThrowInvalidArgError)
{
    RegionOptions opts{DEFAULT_ID, GetParam().width, GetParam().height, DEFAULT_XPOS, DEFAULT_YPOS, DEFAULT_REGION_ZORDER, DEFAULT_REGION_LOOP};

    ASSERT_THROW(RegionBuilder{}.adaptor(unique(&adaptor()))
                                .content(createContentItems(DEFAULT_CONTENT_ITEMS_COUNT))
                                .options(opts)
                                .build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, RegionConstructSizeTest, ::testing::ValuesIn(invalidRegionSizes));

TEST_F(RegionTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructRegion();
}

TEST_F(RegionTest, Construct_Default_HandlerAddChildShouldBeCalled)
{
    EXPECT_CALL(adaptor(), addChild(_, DEFAULT_XPOS, DEFAULT_YPOS));

    constructRegion();
}

TEST_F(RegionTest, Construct_WithoutZorderAndLoop_ZorderLoopEqualsToDefault)
{
    auto region = constructRegion(boost::optional<RegionOptions::Loop>{}, boost::optional<int>{});

    ASSERT_EQ(region->zorder(), DEFAULT_REGION_ZORDER);
    ASSERT_EQ(region->contentLoop(), DEFAULT_REGION_LOOP);
}

TEST_F(RegionTest, Id_Default_EqualsToDefault)
{
    auto region = constructRegion();

    ASSERT_EQ(region->id(), DEFAULT_ID);
}

TEST_F(RegionTest, Loop_Default_EqualsToDefault)
{
    auto region = constructRegion();

    ASSERT_EQ(region->contentLoop(), DEFAULT_REGION_LOOP);
}

TEST_F(RegionTest, Handler_Default_EqualsToPassedAdaptor)
{
    auto region = constructRegion();

    ASSERT_EQ(&region->handler(), &adaptor());
}

TEST_F(RegionTest, WidthAndHeight_Default_RegionWidthAndHeightEqualsDefault)
{
    auto region = constructRegion();

    ASSERT_EQ(region->width(), DEFAULT_WIDTH);
    ASSERT_EQ(region->height(), DEFAULT_HEIGHT);
}

TEST_F(RegionTest, Zorder_Default_RegionZorderEqualsDefault)
{
    auto region = constructRegion();

    ASSERT_EQ(region->zorder(), DEFAULT_REGION_ZORDER);
}

TEST_F(RegionTest, ContentLooped_Enable_RegionContentLoopEnabled)
{
    auto region = constructRegion(RegionOptions::Loop::Enable, DEFAULT_REGION_ZORDER);

    ASSERT_EQ(region->contentLoop(), RegionOptions::Loop::Enable);
}

TEST_F(RegionTest, ContentLooped_Disable_RegionContentLoopEqualsDisabled)
{
    auto region = constructRegion(RegionOptions::Loop::Disable, DEFAULT_REGION_ZORDER);

    ASSERT_EQ(region->contentLoop(), RegionOptions::Loop::Disable);
}

TEST_F(RegionTest, AddContent_Default_RegionShouldSubcribeToDurationExpiredEvent)
{
    auto region = constructRegion();
    auto mockContent = createRegionContent();

    EXPECT_CALL(*mockContent, subscribe(EventType::DurationExpired, _));

    region->addContent(std::move(mockContent), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST_F(RegionTest, Scale_Default_HandlerRegionContentScaleShouldBeCalled)
{
    auto region = constructRegion();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));
    EXPECT_CALL(regionContent(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    region->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(RegionTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto region = constructRegion();

    EXPECT_CALL(adaptor(), show());

    region->show();
}

TEST_F(RegionTest, Show_Default_ContentStartShouldBeCalled)
{
    auto region = constructRegion();

    EXPECT_CALL(regionContent(), start());

    region->show();
}

TEST_F(RegionTest, Show_With2ContentItems_OnlyFirstItemStartShouldBeCalled)
{
    const std::size_t FirstItemIndex = 0;
    const std::size_t ItemsCount = 2;
    auto region = constructRegion(ItemsCount);

    EXPECT_CALL(regionContent(FirstItemIndex), start());
    EXPECT_CALL(regionContent(FirstItemIndex + 1), start()).Times(0);

    region->show();
}
