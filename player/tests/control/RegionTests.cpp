#include "RegionTests.hpp"

using namespace ::testing;

const std::size_t FIRST_CONTENT_INDEX = 0;
const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

const auto invalidRegionSizes = invalidSizes<MIN_WIDTH, MIN_HEIGHT>;

TEST_P(RegionConstructSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    ResourcesXlf::RegionOptions opts{DEFAULT_ID, GetParam().width, GetParam().height, DEFAULT_XPOS, DEFAULT_YPOS, DEFAULT_ZORDER, DEFAULT_LOOP};

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

TEST_F(RegionTest, Construct_WithContent_HandlerAddContentWithPosShouldBeCalled)
{
    EXPECT_CALL(adaptor(), addChild(_, DEFAULT_XPOS, DEFAULT_YPOS));

    constructRegion();
}

TEST_F(RegionTest, Constructor_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructRegion();
}

TEST_F(RegionTest, Id_Default_EqualsToDefaultId)
{
    auto region = constructRegion();

    ASSERT_EQ(region->id(), DEFAULT_ID);
}

TEST_F(RegionTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto region = constructRegion();

    ASSERT_EQ(&region->handler(), &adaptor());
}

TEST_F(RegionTest, Width_HandlerReturnsDefaultWidth_RegionWidthEqualsDefault)
{
    auto region = constructRegion();

    ON_CALL(adaptor(), width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(region->width(), DEFAULT_WIDTH);
}

TEST_F(RegionTest, Height_HandlerReturnsDefaultHeight_RegionHeightEqualsDefault)
{
    auto region = constructRegion();

    ON_CALL(adaptor(), height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(region->height(), DEFAULT_HEIGHT);
}

TEST_F(RegionTest, Zorder_Default_RegionZorderEquals0)
{
    auto region = constructRegion();

    ASSERT_EQ(region->zorder(), DEFAULT_ZORDER);
}

TEST_F(RegionTest, ContentLooped_Default_RegionContentLoopedEqualsFalse)
{
    auto region = constructRegion();

    ASSERT_EQ(region->contentLooped(), false);
}

TEST_F(RegionTest, ContentLooped_LoopedContent_RegionContentLoopedEqualsFalse)
{
    auto region = constructRegion();

    region->loopContent();

    ASSERT_EQ(region->contentLooped(), true);
}

TEST_F(RegionTest, AddContentWithCoords_Valid_RegionShouldSubcribeToDurationExpiredEvent)
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
    EXPECT_CALL(regionContent(FIRST_CONTENT_INDEX), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    region->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(RegionTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto region = constructRegion();

    EXPECT_CALL(adaptor(), show());

    region->show();
}

TEST_F(RegionTest, Show_With2ContentItems_FirstItemStartShouldBeCalled)
{
    const std::size_t ItemsCount = 2;
    auto region = constructRegion(ItemsCount);

    EXPECT_CALL(regionContent(FIRST_CONTENT_INDEX), start());
    EXPECT_CALL(regionContent(FIRST_CONTENT_INDEX + 1), start()).Times(0);

    region->show();
}
