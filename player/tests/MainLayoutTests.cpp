#include "test_utils.hpp"

#include "control/MainLayout.hpp"

#include "MockOverlayAdaptor.hpp"
#include "MockBackground.hpp"
#include "MockImageAdaptor.hpp"
#include "MockRegion.hpp"
#include "MockFixedLayoutAdaptor.hpp"

using namespace ::testing;

auto construct_layout(int width, int height)
{
    return construct<MainLayout, MockOverlayAdaptor>(width, height);
}

auto construct_layout()
{
    return construct<MainLayout, MockOverlayAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

NiceMock<MockBackground>* createBackground()
{
    auto background = new NiceMock<MockBackground>;
    auto stubImage = std::make_unique<NiceMock<MockImageAdaptor>>();

    ON_CALL(*background, handler()).WillByDefault(ReturnRef(*stubImage));

    return background;
}

NiceMock<MockRegion>* createRegion()
{
    auto region = new NiceMock<MockRegion>;
    auto stubFixed = std::make_unique<NiceMock<MockFixedLayoutAdaptor>>();

    ON_CALL(*region, handler()).WillByDefault(ReturnRef(*stubFixed));

    return region;
}

class MainLayoutTest : public TestWithParam<Size> { };

TEST_P(MainLayoutTest, Constructor_InvalidSize_ShouldThrowRunTimeError)
{
    ASSERT_THROW(construct_layout(GetParam().width, GetParam().height), std::runtime_error);
}

TEST_P(MainLayoutTest, SetSize_InvalidSize_ShouldThrowRunTimeError)
{
    auto [background, stubImage] = construct_layout();

    ASSERT_THROW(background->setSize(GetParam().width, GetParam().height), std::runtime_error);
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutTest, ::testing::ValuesIn(invalidSizes));

TEST(MainLayoutTest, Constructor_Default_HandlerSetSizeShouldBeCalled)
{
    auto mockOverlay = std::make_unique<NiceMock<MockOverlayAdaptor>>();

    EXPECT_CALL(*mockOverlay, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    std::make_shared<MainLayout>(DEFAULT_WIDTH, DEFAULT_HEIGHT, std::move(mockOverlay));
}

TEST(MainLayoutTest, Width_Default_Equals640)
{
    auto [layout, stubOverlay] = construct_layout();

    ON_CALL(*stubOverlay, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
}

TEST(MainLayoutTest, Height_Default_Equals480)
{
    auto [layout, stubOverlay] = construct_layout();

    ON_CALL(*stubOverlay, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, Background_NotSet_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct_layout();

    ASSERT_THROW(layout->background(), std::runtime_error);
}

TEST(MainLayoutTest, SetBackground_Null_HandlerAddShouldNotBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, addMainChild(_)).Times(0);

    layout->setBackground(nullptr);
}

TEST(MainLayoutTest, SetBackground_NotNull_HandlerAddShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, addMainChild(_));

    layout->setBackground(unique(createBackground()));
}

TEST(MainLayoutTest, SetBackground_NotNull_BackgroundShouldReturnExistingObject)
{
    auto [layout, stubOverlay] = construct_layout();
    auto stubBackground = createBackground();

    layout->setBackground(unique(stubBackground));

    ASSERT_EQ(&layout->background(), stubBackground);
}

TEST(MainLayoutTest, SetBackground_WidthNotEqualLayotuWidth_RunTimeErrorShouldBeCalled)
{
    auto [layout, stubOverlay] = construct_layout();
    auto stubBackground = createBackground();

    ON_CALL(*stubOverlay, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->setBackground(unique(stubBackground)), std::runtime_error);
}

TEST(MainLayoutTest, SetBackground_HeightNotEqualLayotHeight_RunTimeErrorShouldBeCalled)
{
    auto [layout, stubOverlay] = construct_layout();
    auto stubBackground = createBackground();

    ON_CALL(*stubOverlay, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->setBackground(unique(stubBackground)), std::runtime_error);
}

TEST(MainLayoutTest, SetBackground_2InRow_HandlerAdd2TimesRemove1Time)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, addMainChild(_)).Times(2);
    EXPECT_CALL(*mockOverlay, removeMainChild()).Times(1);

    layout->setBackground(unique(createBackground()));
    layout->setBackground(unique(createBackground()));
}

TEST(MainLayoutTest, SetSize_Width200Height200_HandlerSetSizeShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, SetSize_Width200Height200_WidthEquals200)
{
    auto [layout, stubOverlay] = construct_layout();

    ON_CALL(*stubOverlay, width()).WillByDefault(Return(NEW_DEFAULT_WIDTH));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(layout->width(), NEW_DEFAULT_WIDTH);
}

TEST(MainLayoutTest, SetSize_Width200Height200_HeightEquals200)
{
    auto [layout, stubOverlay] = construct_layout();

    ON_CALL(*stubOverlay, height()).WillByDefault(Return(NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(layout->height(), NEW_DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, SetSize_WithBackground_BackgroundSetSizeShouldBeCalled)
{
    auto [layout, stubOverlay] = construct_layout();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, AddRegion_Null_HandlerAddChildShouldNotBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, addChild(_, _, _ ,_ , _)).Times(0);

    layout->addRegion(nullptr);
}

TEST(MainLayoutTest, AddRegion_Add1_HandlerAddChildShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, addChild(_, _, _ ,_ , _));

    layout->addRegion(unique(createRegion()));
}

TEST(MainLayoutTest, AddRegion_RegionWidthGreaterThanLayoutWidth_RunTimeErrorShouldBeThrown)
{
    auto [layout, stubOverlay] = construct_layout();
    auto stubRegion = createRegion();

    ON_CALL(*stubRegion, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->addRegion(unique(stubRegion)), std::runtime_error);
}

TEST(MainLayoutTest, AddRegion_RegionHeightGreaterThanLayoutWidth_RunTimeErrorShouldBeThrown)
{
    auto [layout, stubOverlay] = construct_layout();
    auto stubRegion = createRegion();

    ON_CALL(*stubRegion, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->addRegion(unique(stubRegion)), std::runtime_error);
}

TEST(MainLayoutTest, RemoveAllRegions_With3Regions_RegionsCountEquals0)
{
    auto [layout, stubOverlay] = construct_layout();
    const int REGIONS_COUNT = 3;
    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        layout->addRegion(unique(createRegion()));
    }

    layout->removeAllRegions();

    ASSERT_EQ(layout->regionsCount(), 0);
}

TEST(MainLayoutTest, RemoveAllRegions_With3Regions_HandlerRemoveChildrenShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();
    const int REGIONS_COUNT = 3;
    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        layout->addRegion(unique(createRegion()));
    }

    EXPECT_CALL(*mockOverlay, removeChildren());

    layout->removeAllRegions();
}

TEST(MainLayoutTest, Region_DefaultGet0Element_ShouldThrowOutOfRangeException)
{
    auto [layout, stubOverlay] = construct_layout();

    ASSERT_THROW(layout->region(0), std::out_of_range);
}

TEST(MainLayoutTest, Region_Add1RegionAndGet1_ShouldThrowOutOfRangeException)
{
    auto [layout, stubOverlay] = construct_layout();

    layout->addRegion(unique(createRegion()));

    ASSERT_THROW(layout->region(1), std::out_of_range);
}

TEST(MainLayoutTest, Region_Add1RegionAndGetNegative_ShouldThrowOutOfRangeException)
{
    auto [layout, stubOverlay] = construct_layout();

    layout->addRegion(unique(createRegion()));

    ASSERT_THROW(layout->region(-10), std::out_of_range);
}

TEST(MainLayoutTest, Region_Add1RegionAndGetBigIndex_ShouldThrowOutOfRangeException)
{
    auto [layout, stubOverlay] = construct_layout();

    layout->addRegion(unique(createRegion()));

    ASSERT_THROW(layout->region(std::numeric_limits<size_t>::max()), std::out_of_range);
}

TEST(MainLayoutTest, Region_Add1RegionAndGet0_ShouldReturnAddedRegion)
{
    auto [layout, stubOverlay] = construct_layout();

    auto stubRegion = createRegion();
    layout->addRegion(unique(stubRegion));

    ASSERT_EQ(&layout->region(0), stubRegion);
}

TEST(MainLayoutTest, RegionsCount_Default_Equals0)
{
    auto [layout, stubOverlay] = construct_layout();

    ASSERT_EQ(layout->regionsCount(), 0);
}

TEST(MainLayoutTest, RegionsCount_Add3Regions_RegionCountEquals3)
{
    auto [layout, stubOverlay] = construct_layout();
    const int REGIONS_COUNT = 3;
    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        layout->addRegion(unique(createRegion()));
    }

    ASSERT_EQ(layout->regionsCount(), REGIONS_COUNT);
}

TEST(MainLayoutTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, show());

    layout->show();
}

TEST(MainLayoutTest, Show_WithBackground_BackgroundShowShouldBeCalled)
{
    auto [layout, stubOverlay] = construct_layout();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, show());

    layout->show();
}

TEST(MainLayoutTest, Show_WithRegion_RegionShowShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();
    auto mockRegion = createRegion();
    layout->addRegion(unique(mockRegion));

    EXPECT_CALL(*mockRegion, show());

    layout->show();
}

TEST(MainLayoutTest, Show_With3Regions_HandlerReorderChildShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();
    const int REGIONS_COUNT = 3;
    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        layout->addRegion(unique(createRegion()));
    }

    EXPECT_CALL(*mockOverlay, reorderChild(_, _)).Times(REGIONS_COUNT);

    layout->show();
}
