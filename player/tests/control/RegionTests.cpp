#include "RegionTests.hpp"

using namespace ::testing;

const std::size_t FIRST_CONTENT_INDEX = 0;

TEST_F(RegionTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto region = constructRegion();

    ASSERT_EQ(&region->handler(), &adaptor());
}

TEST_F(RegionTest, Id_Default_IdEqualsDefault)
{
    auto region = constructRegion();

    ASSERT_EQ(region->id(), DEFAULT_ID);
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

TEST_F(RegionTest, AddContentWithCoords_Valid_HandlerAddChildShouldBeCalled)
{
    auto region = constructRegion();

    EXPECT_CALL(adaptor(), addChild(_, DEFAULT_XPOS, DEFAULT_YPOS));

    region->addContent(createRegionContent(), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST_F(RegionTest, AddContentWithCoords_Valid_RegionShouldSubcribeToDurationExpiredEvent)
{
    auto region = constructRegion();
    auto mockContent = createRegionContent();

    EXPECT_CALL(*mockContent, subscribe(EventType::DurationExpired, _));

    region->addContent(std::move(mockContent), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST_F(RegionTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto region = constructRegion();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    region->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(RegionTest, Scale_MediaWithPos_ContentScaleShouldBeCalled)
{
    auto region = constructRegion();

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
