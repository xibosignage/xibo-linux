#include "control/MainLayout.hpp"
#include "media/IMedia.hpp"

#include "MockOverlayAdaptor.hpp"
#include "MockBackground.hpp"
#include "MockImageAdaptor.hpp"
#include "MockRegion.hpp"
#include "MockFixedLayoutAdaptor.hpp"

using namespace ::testing;

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

const int NEW_DEFAULT_WIDTH = 200;
const int NEW_DEFAULT_HEIGHT = 200;

std::pair<std::shared_ptr<MainLayout>, NiceMock<MockOverlayAdaptor>*>
construct(int width, int height)
{
    auto overlay = std::make_unique<NiceMock<MockOverlayAdaptor>>();
    auto overlayRaw = overlay.get();
    auto layout = std::make_shared<MainLayout>(width, height, std::move(overlay));
    return std::pair{layout, overlayRaw};
}

std::pair<std::shared_ptr<MainLayout>, NiceMock<MockOverlayAdaptor>*>
construct()
{
    auto overlay = std::make_unique<NiceMock<MockOverlayAdaptor>>();
    auto overlayRaw = overlay.get();
    auto layout = std::make_shared<MainLayout>(DEFAULT_WIDTH, DEFAULT_HEIGHT, std::move(overlay));
    return std::pair{layout, overlayRaw};
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

template<typename T>
std::unique_ptr<T> unique(T* pointer)
{
    return std::unique_ptr<T>{pointer};
}

TEST(MainLayout, Constructor_NegativeWidth_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct(-1, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, Constructor_NegativeHeigth_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct(DEFAULT_WIDTH, -1), std::runtime_error);
}

TEST(MainLayout, Constructor_WidthGreaterThanMaxWidth9999_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct(10000, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, Constructor_HeightGreaterThanMaxHeight9999_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct(DEFAULT_WIDTH, 10000), std::runtime_error);
}

TEST(MainLayout, Constructor_WidthLesshanMinWidth160_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct(159, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, Constructor_HeightLessThanMinHeight120_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct(DEFAULT_WIDTH, 119), std::runtime_error);
}

TEST(MainLayout, Constructor_VeryBigWidth_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct(std::numeric_limits<int>::max(), DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, Constructor_VeryBigHeight_ShouldThrowRunTimeError)
{
    EXPECT_THROW(construct(DEFAULT_WIDTH, std::numeric_limits<int>::max()), std::runtime_error);
}

TEST(MainLayout, Constructor_Default_HandlerSetSizeShouldBeCalled)
{
    auto mockOverlay = std::make_unique<NiceMock<MockOverlayAdaptor>>();

    EXPECT_CALL(*mockOverlay, setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    std::make_shared<MainLayout>(DEFAULT_WIDTH, DEFAULT_HEIGHT, std::move(mockOverlay));
}

TEST(MainLayout, Width_Default_Equals640)
{
    auto [layout, stubOverlay] = construct();

    ON_CALL(*stubOverlay, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
}

TEST(MainLayout, Height_Default_Equals480)
{
    auto [layout, stubOverlay] = construct();

    ON_CALL(*stubOverlay, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST(MainLayout, Background_NotSet_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->background(), std::runtime_error);
}

TEST(MainLayout, SetBackground_Null_HandlerAddShouldNotBeCalled)
{
    auto [layout, mockOverlay] = construct();

    EXPECT_CALL(*mockOverlay, addMainChild(_)).Times(0);

    layout->setBackground(nullptr);
}

TEST(MainLayout, SetBackground_NotNull_HandlerAddShouldBeCalled)
{
    auto [layout, mockOverlay] = construct();

    EXPECT_CALL(*mockOverlay, addMainChild(_));

    layout->setBackground(unique(createBackground()));
}

TEST(MainLayout, SetBackground_NotNull_BackgroundShouldReturnExistingObject)
{
    auto [layout, stubOverlay] = construct();
    auto stubBackground = createBackground();

    layout->setBackground(unique(stubBackground));

    ASSERT_EQ(&layout->background(), stubBackground);
}

TEST(MainLayout, SetBackground_WidthNotEqualLayotuWidth_RunTimeErrorShouldBeCalled)
{
    auto [layout, stubOverlay] = construct();
    auto stubBackground = createBackground();

    ON_CALL(*stubOverlay, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->setBackground(unique(stubBackground)), std::runtime_error);
}

TEST(MainLayout, SetBackground_HeightNotEqualLayotHeight_RunTimeErrorShouldBeCalled)
{
    auto [layout, stubOverlay] = construct();
    auto stubBackground = createBackground();

    ON_CALL(*stubOverlay, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->setBackground(unique(stubBackground)), std::runtime_error);
}

TEST(MainLayout, SetBackground_2InRow_HandlerAdd2TimesRemove1Time)
{
    auto [layout, mockOverlay] = construct();

    EXPECT_CALL(*mockOverlay, addMainChild(_)).Times(2);
    EXPECT_CALL(*mockOverlay, removeMainChild()).Times(1);

    layout->setBackground(unique(createBackground()));
    layout->setBackground(unique(createBackground()));
}

TEST(MainLayout, SetSize_Width200Height200_HandlerSetSizeShouldBeCalled)
{
    auto [layout, mockOverlay] = construct();

    EXPECT_CALL(*mockOverlay, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayout, SetSize_Width200Height200_WidthEquals200)
{
    auto [layout, stubOverlay] = construct();

    ON_CALL(*stubOverlay, width()).WillByDefault(Return(NEW_DEFAULT_WIDTH));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(layout->width(), NEW_DEFAULT_WIDTH);
}

TEST(MainLayout, SetSize_Width200Height200_HeightEquals200)
{
    auto [layout, stubOverlay] = construct();

    ON_CALL(*stubOverlay, height()).WillByDefault(Return(NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(layout->height(), NEW_DEFAULT_HEIGHT);
}

TEST(MainLayout, SetSize_NegativeWidth_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->setSize(-1, NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, SetSize_NegativeHeigth_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->setSize(NEW_DEFAULT_WIDTH, -1), std::runtime_error);
}

TEST(MainLayout, SetSize_WidthGreaterThan9999_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->setSize(10000, NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, SetSize_HeightGreaterThan9999_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->setSize(NEW_DEFAULT_WIDTH, 10000), std::runtime_error);
}

TEST(MainLayout, SetSize_WidthLesshanMinWidth160_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->setSize(159, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, SetSize_HeightLessThanMinHeight120_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->setSize(DEFAULT_WIDTH, 119), std::runtime_error);
}

TEST(MainLayout, SetSize_VeryBigWidth_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->setSize(std::numeric_limits<int>::max(), NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, SetSize_VeryBigHeight_ShouldThrowRunTimeError)
{
    auto [layout, stubOverlay] = construct();

    EXPECT_THROW(layout->setSize(NEW_DEFAULT_WIDTH, std::numeric_limits<int>::max()), std::runtime_error);
}

TEST(MainLayout, SetSize_WithBackground_BackgroundSetSizeShouldBeCalled)
{
    auto [layout, stubOverlay] = construct();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayout, AddRegion_Null_HandlerAddChildShouldNotBeCalled)
{
    auto [layout, mockOverlay] = construct();

    EXPECT_CALL(*mockOverlay, addChild(_, _, _ ,_ , _)).Times(0);

    layout->addRegion(nullptr);
}

TEST(MainLayout, AddRegion_Add1_HandlerAddChildShouldBeCalled)
{
    auto [layout, mockOverlay] = construct();

    EXPECT_CALL(*mockOverlay, addChild(_, _, _ ,_ , _));

    layout->addRegion(unique(createRegion()));
}

TEST(MainLayout, AddRegion_RegionWidthGreaterThanLayoutWidth_RunTimeErrorShouldBeThrown)
{
    auto [layout, stubOverlay] = construct();
    auto stubRegion = createRegion();

    ON_CALL(*stubRegion, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    EXPECT_THROW(layout->addRegion(unique(stubRegion)), std::runtime_error);
}

TEST(MainLayout, AddRegion_RegionHeightGreaterThanLayoutWidth_RunTimeErrorShouldBeThrown)
{
    auto [layout, stubOverlay] = construct();
    auto stubRegion = createRegion();

    ON_CALL(*stubRegion, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    EXPECT_THROW(layout->addRegion(unique(stubRegion)), std::runtime_error);
}

TEST(MainLayout, RemoveAllRegions_With3Regions_RegionsCountEquals0)
{
    auto [layout, stubOverlay] = construct();
    const int REGIONS_COUNT = 3;
    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        layout->addRegion(unique(createRegion()));
    }

    layout->removeAllRegions();

    ASSERT_EQ(layout->regionsCount(), 0);
}

TEST(MainLayout, RemoveAllRegions_With3Regions_HandlerRemoveChildrenShouldBeCalled)
{
    auto [layout, mockOverlay] = construct();
    const int REGIONS_COUNT = 3;
    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        layout->addRegion(unique(createRegion()));
    }

    EXPECT_CALL(*mockOverlay, removeChildren());

    layout->removeAllRegions();
}

TEST(MainLayout, Region_DefaultGet0Element_ShouldThrowOutOfRangeException)
{
    auto [layout, stubOverlay] = construct();

    ASSERT_THROW(layout->region(0), std::out_of_range);
}

TEST(MainLayout, Region_Add1RegionAndGet1_ShouldThrowOutOfRangeException)
{
    auto [layout, stubOverlay] = construct();

    layout->addRegion(unique(createRegion()));

    ASSERT_THROW(layout->region(1), std::out_of_range);
}

TEST(MainLayout, Region_Add1RegionAndGetNegative_ShouldThrowOutOfRangeException)
{
    auto [layout, stubOverlay] = construct();

    layout->addRegion(unique(createRegion()));

    ASSERT_THROW(layout->region(-10), std::out_of_range);
}

TEST(MainLayout, Region_Add1RegionAndGetBigIndex_ShouldThrowOutOfRangeException)
{
    auto [layout, stubOverlay] = construct();

    layout->addRegion(unique(createRegion()));

    ASSERT_THROW(layout->region(std::numeric_limits<size_t>::max()), std::out_of_range);
}

TEST(MainLayout, Region_Add1RegionAndGet0_ShouldReturnAddedRegion)
{
    auto [layout, stubOverlay] = construct();

    auto stubRegion = createRegion();
    layout->addRegion(unique(stubRegion));

    ASSERT_EQ(&layout->region(0), stubRegion);
}

TEST(MainLayout, RegionsCount_Default_Equals0)
{
    auto [layout, stubOverlay] = construct();

    ASSERT_EQ(layout->regionsCount(), 0);
}

TEST(MainLayout, RegionsCount_Add3Regions_RegionCountEquals3)
{
    auto [layout, stubOverlay] = construct();
    const int REGIONS_COUNT = 3;
    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        layout->addRegion(unique(createRegion()));
    }

    ASSERT_EQ(layout->regionsCount(), REGIONS_COUNT);
}

TEST(MainLayout, Show_Default_HandlerShowShouldBeCalled)
{
    auto [layout, mockOverlay] = construct();

    EXPECT_CALL(*mockOverlay, show());

    layout->show();
}

TEST(MainLayout, Show_WithBackground_BackgroundShowShouldBeCalled)
{
    auto [layout, stubOverlay] = construct();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, show());

    layout->show();
}

TEST(MainLayout, Show_WithRegion_RegionShowShouldBeCalled)
{
    auto [layout, mockOverlay] = construct();
    auto mockRegion = createRegion();
    layout->addRegion(unique(mockRegion));

    EXPECT_CALL(*mockRegion, show());

    layout->show();
}

TEST(MainLayout, Show_With3Regions_HandlerReorderChildShouldBeCalled)
{
    auto [layout, mockOverlay] = construct();
    const int REGIONS_COUNT = 3;
    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        layout->addRegion(unique(createRegion()));
    }

    EXPECT_CALL(*mockOverlay, reorderChild(_, _)).Times(REGIONS_COUNT);

    layout->show();
}
