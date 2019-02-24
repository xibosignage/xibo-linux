#include "MainLayoutTests.hpp"

using namespace ::testing;

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

const auto invalidMainLayoutSizes = invalidSizes<MIN_WIDTH, MIN_HEIGHT>;

const std::vector<std::vector<int>> zorders = {
    {10,9,8},
    {1,2,4,10},
    {10,7,9,1},
    {5,4},
    {1},
    {4,1,5,2},
    {4,3,2,1,0}
};

TEST_P(MainLayoutSizeTest, Construct_InvalidSize_ShouldThrowInvalidArgError)
{
    LayoutOptions opts{GetParam().width, GetParam().height};

    ASSERT_THROW(MainLayoutBuilder{}.adaptor(unique(&adaptor()))
                                    .options(opts)
                                    .background(createBackground())
                                    .regions(createRegions(DEFAULT_REGIONS_COUNT))
                                    .build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutSizeTest, ::testing::ValuesIn(invalidMainLayoutSizes));

TEST_F(MainLayoutTest, Construct_WithoutRegions_ShouldThrowRunTimeError)
{
    const int regionsCount = 0;

    ASSERT_THROW(constructLayout(regionsCount), std::runtime_error);
}

TEST_F(MainLayoutTest, Construct_Default_HandlerAddChildShouldBeCalled)
{
    EXPECT_CALL(adaptor(), addChild(_, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_XPOS, DEFAULT_YPOS)).Times(DEFAULT_REGIONS_COUNT);

    constructLayout();
}

TEST_F(MainLayoutTest, Construct_Default_HandlerAddMainChildShouldCalled)
{
    EXPECT_CALL(adaptor(), addMainChild(_));

    constructLayout();
}

TEST_F(MainLayoutTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructLayout();
}

TEST_F(MainLayoutTest, Handler_Default_EqualsToPassedAdaptor)
{
    auto layout = constructLayout();

    ASSERT_EQ(&layout->handler(), &adaptor());
}

TEST_F(MainLayoutTest, WidthAndHeight_Default_LayoutWidthAndHeightEqualsDefault)
{
    auto layout = constructLayout();

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST_F(MainLayoutTest, SetBackground_BackgroundWidthNotEqualsLayoutWidth_InvalidArgErrorShouldBeThrown)
{
    auto layout = constructLayout();
    auto stubBackground = createBackground();

    ON_CALL(*stubBackground, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->setBackground(std::move(stubBackground)), std::invalid_argument);
}

TEST_F(MainLayoutTest, SetBackground_BackgroundHeightNotEqualsLayoutHeight_InvalidArgErrorShouldBeThrown)
{
    auto layout = constructLayout();
    auto stubBackground = createBackground();

    ON_CALL(*stubBackground, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->setBackground(std::move(stubBackground)), std::invalid_argument);
}

TEST_F(MainLayoutTest, SetBackground_AlreadySetBefore_HandlerAddMainChildCalledOnceRemoveMainChildCalledOnce)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), addMainChild(_)).Times(1);
    EXPECT_CALL(adaptor(), removeMainChild()).Times(1);

    layout->setBackground(createBackground());
}

TEST_F(MainLayoutTest, AddRegion_RegionWidthGreaterThanLayoutWidth_InvalidArgErrorShouldBeThrown)
{
    auto layout = constructLayout();
    auto stubRegion = createRegion();

    ON_CALL(*stubRegion, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->addRegion(std::move(stubRegion), DEFAULT_XPOS, DEFAULT_YPOS), std::invalid_argument);
}

TEST_F(MainLayoutTest, AddRegion_RegionHeightGreaterThanLayoutHeight_InvalidArgErrorShouldBeThrown)
{
    auto layout = constructLayout();
    auto stubRegion = createRegion();

    ON_CALL(*stubRegion, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->addRegion(std::move(stubRegion), DEFAULT_XPOS, DEFAULT_YPOS), std::invalid_argument);
}

TEST_F(MainLayoutTest, Scale_Default_HandlerRegionBackgroundScaleShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));
    EXPECT_CALL(region(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));
    EXPECT_CALL(background(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    layout->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(MainLayoutTest, Show_NotShownBefore_HandlerRegionBackgroundShowShouldBeCalled)
{
    auto layout = constructLayout();

    ON_CALL(adaptor(), isShown()).WillByDefault(Return(false));

    EXPECT_CALL(adaptor(), show());
    EXPECT_CALL(background(), show());
    EXPECT_CALL(region(), show());

    layout->show();
}

TEST_F(MainLayoutTest, Show_ShownBefore_HandlerRegionBackgroundShowShouldNotBeCalled)
{
    auto layout = constructLayout();

    ON_CALL(adaptor(), isShown()).WillByDefault(Return(true));

    EXPECT_CALL(background(), show()).Times(0);
    EXPECT_CALL(region(), show()).Times(0);
    EXPECT_CALL(adaptor(), show()).Times(0);

    layout->show();
}

TEST_P(MainLayoutReorderTest, Show_Default_HandlerReorderChildShouldBeCalledWithCorrectOrder)
{
    auto layout = constructLayout();

    for(size_t i = 0; i != GetParam().size(); ++i)
    {
        EXPECT_CALL(adaptor(), reorderChild(Ref(region(i).handler()), static_cast<int>(i)));
    }

    layout->show();
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutReorderTest, ::testing::ValuesIn(zorders));
