#include "MainLayoutTests.hpp"

using namespace ::testing;

const int MIN_WIDTH = 1;
const int MIN_HEIGHT = 1;

const auto invalidMainLayoutSizes = invalidSizes<MIN_WIDTH, MIN_HEIGHT>;

TEST_P(MainLayoutConstructSizeTest, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    LayoutOptions opts{DEFAULT_SCHEME, GetParam().width, GetParam().height};

    ASSERT_THROW(MainLayoutBuilder{}.adaptor(unique(&adaptor()))
                                    .options(opts)
                                    .background(createBackground())
                                    .regions(createRegions(DEFAULT_REGIONS_COUNT))
                                    .build(), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutConstructSizeTest, ::testing::ValuesIn(invalidMainLayoutSizes));

TEST_F(MainLayoutTest, Construct_WithoutRegions_ShouldThrowRunTimeError)
{
    const int regionsCount = 0;

    ASSERT_THROW(constructLayout(regionsCount), std::runtime_error);
}

TEST_F(MainLayoutTest, Construct_WithContinaer_HandlerAddRegionWithPos)
{
    EXPECT_CALL(adaptor(), addChild(_, _, _, _, _)).Times(DEFAULT_REGIONS_COUNT);

    constructLayout();
}

TEST_F(MainLayoutTest, Construct_WithBackground_HandlerSetBackground)
{
    EXPECT_CALL(adaptor(), addMainChild(_));

    constructLayout();
}

TEST_F(MainLayoutTest, Construct_Default_HandlerSetSizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    constructLayout();
}

TEST_F(MainLayoutTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto layout = constructLayout();

    ASSERT_EQ(&layout->handler(), &adaptor());
}

TEST_F(MainLayoutTest, Width_HandlerReturnsDefaulSize_LayoutWidthSizeDefault)
{
    auto layout = constructLayout();

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST_F(MainLayoutTest, SetBackground_BackgroundWidthNotEqualLayoutWidth_InvalidArgErrorShouldBeCalled)
{
    auto layout = constructLayout();
    auto stubBackground = createBackground();

    ON_CALL(*stubBackground, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->setBackground(std::move(stubBackground)), std::invalid_argument);
}

TEST_F(MainLayoutTest, SetBackground_BackgroundHeightNotEqualLayoutHeight_InvalidArgErrorShouldBeCalled)
{
    auto layout = constructLayout();
    auto stubBackground = createBackground();

    ON_CALL(*stubBackground, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->setBackground(std::move(stubBackground)), std::invalid_argument);
}

TEST_F(MainLayoutTest, SetBackground_AlreadySetBefore_HandlerAddMainChild1TimeCalledRemoveMainChild1TimeCalled)
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

TEST_F(MainLayoutTest, AddRegion_RegionHeightGreaterThanLayoutWidth_InvalidArgErrorShouldBeThrown)
{
    auto layout = constructLayout();
    auto stubRegion = createRegion();

    ON_CALL(*stubRegion, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->addRegion(std::move(stubRegion), DEFAULT_XPOS, DEFAULT_YPOS), std::invalid_argument);
}

TEST_F(MainLayoutTest, Scale_Default_AdaptorRegionBackgroundScaleShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));
    EXPECT_CALL(region(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));
    EXPECT_CALL(background(), scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    layout->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST_F(MainLayoutTest, Show_Once_AdaptorRegionBackgroundShowShouldBeCalled)
{
    auto layout = constructLayout();

    EXPECT_CALL(adaptor(), show());
    EXPECT_CALL(background(), show());
    EXPECT_CALL(region(), show());

    ON_CALL(adaptor(), isShown()).WillByDefault(Return(false));
    layout->show();
}

TEST_F(MainLayoutTest, Show_Twice_AdaptorRegionBackgroundShowShouldBeCalledOnce)
{
    auto layout = constructLayout();

    EXPECT_CALL(background(), show()).Times(1);
    EXPECT_CALL(region(), show()).Times(1);
    EXPECT_CALL(adaptor(), show()).Times(1);

    layout->show();
    ON_CALL(adaptor(), isShown()).WillByDefault(Return(true));
    layout->show();
}

TEST_P(MainLayoutReorderTest, Show_With3Regions_HandlerReorderChildShouldBeCalledWithCorrectOrder)
{
    auto layout = constructLayout();

    for(size_t index = 0; index != GetParam().size(); ++index)
    {
        EXPECT_CALL(adaptor(), reorderChild(Ref(region(index).handler()), static_cast<int>(index)));
    }

    layout->show();
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutReorderTest, ::testing::ValuesIn(zorders));
