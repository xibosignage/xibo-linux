#include "control/MainLayout.hpp"
#include "media/IMedia.hpp"

#include "MockOverlayWrapper.hpp"
#include "MockBackground.hpp"
#include "MockImageWrapper.hpp"
#include "MockRegion.hpp"
#include "MockFixedLayoutWrapper.hpp"

using namespace ::testing;

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

const int NEW_DEFAULT_WIDTH = 200;
const int NEW_DEFAULT_HEIGHT = 200;

std::pair<std::shared_ptr<MainLayout>, std::shared_ptr<NiceMock<MockOverlayWrapper>>>
construct(int width, int height)
{
    auto overlay = std::make_shared<NiceMock<MockOverlayWrapper>>();
    auto layout = std::make_shared<MainLayout>(width, height, overlay);
    return std::pair{layout, overlay};
}

std::pair<std::shared_ptr<MainLayout>, std::shared_ptr<NiceMock<MockOverlayWrapper>>>
construct()
{
    auto overlay = std::make_shared<NiceMock<MockOverlayWrapper>>();
    auto layout = std::make_shared<MainLayout>(DEFAULT_WIDTH, DEFAULT_HEIGHT, overlay);
    return std::pair{layout, overlay};
}

std::shared_ptr<NiceMock<MockBackground>> create_background()
{
    auto background = std::make_shared<NiceMock<MockBackground>>();
    auto stub_image = std::make_shared<NiceMock<MockImageWrapper>>();

    ON_CALL(*background, handler()).WillByDefault(ReturnRef(*stub_image));

    return background;
}

std::shared_ptr<NiceMock<MockRegion>> add_region(MainLayout& layout)
{
    auto region = std::make_shared<NiceMock<MockRegion>>();
    auto stub_fixed = std::make_shared<NiceMock<MockFixedLayoutWrapper>>();

    ON_CALL(*region, handler()).WillByDefault(ReturnRef(*stub_fixed));

    layout.add_region(region);

    return region;
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
    auto mock_overlay = std::make_shared<NiceMock<MockOverlayWrapper>>();

    EXPECT_CALL(*mock_overlay, set_size(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    std::make_shared<MainLayout>(DEFAULT_WIDTH, DEFAULT_HEIGHT, mock_overlay);
}

TEST(MainLayout, Width_Default_Equals640)
{
    auto [layout, stub_overlay] = construct();

    ON_CALL(*stub_overlay, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
}

TEST(MainLayout, Height_Default_Equals480)
{
    auto [layout, stub_overlay] = construct();

    ON_CALL(*stub_overlay, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST(MainLayout, Background_Default_ShouldThrowRunTimeException)
{
    auto [layout, stub_overlay] = construct();

    ASSERT_THROW(layout->background(), std::runtime_error);
}

TEST(MainLayout, SetBackground_Null_HandlerAddShouldNotBeCalled)
{
    auto [layout, mock_overlay] = construct();

    EXPECT_CALL(*mock_overlay, add(_)).Times(0);

    layout->set_background(nullptr);
}

TEST(MainLayout, SetBackground_NotNull_HandlerAddShouldBeCalled)
{
    auto [layout, mock_overlay] = construct();

    EXPECT_CALL(*mock_overlay, add(_));

    layout->set_background(create_background());
}

TEST(MainLayout, SetBackground_NotNull_BackgroundSetSizeShouldBeCalled)
{
    auto [layout, stub_overlay] = construct();
    auto mock_background = create_background();

    ON_CALL(*stub_overlay, width()).WillByDefault(Return(DEFAULT_WIDTH));
    ON_CALL(*stub_overlay, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    EXPECT_CALL(*mock_background, set_size(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    layout->set_background(mock_background);
}

TEST(MainLayout, SetBackground_2InRow_HandlerAdd2TimesRemove1Time)
{
    auto [layout, mock_overlay] = construct();

    EXPECT_CALL(*mock_overlay, add(_)).Times(2);
    EXPECT_CALL(*mock_overlay, remove()).Times(1);

    layout->set_background(create_background());
    layout->set_background(create_background());
}

TEST(MainLayout, SetSize_Width200Height200_HandlerSetSizeShouldBeCalled)
{
    auto [layout, mock_overlay] = construct();

    EXPECT_CALL(*mock_overlay, set_size(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->set_size(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayout, SetSize_Width200Height200_WidthEquals200)
{
    auto [layout, stub_overlay] = construct();

    ON_CALL(*stub_overlay, width()).WillByDefault(Return(NEW_DEFAULT_WIDTH));

    layout->set_size(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(layout->width(), NEW_DEFAULT_WIDTH);
}

TEST(MainLayout, SetSize_Width200Height200_HeightEquals200)
{
    auto [layout, stub_overlay] = construct();

    ON_CALL(*stub_overlay, height()).WillByDefault(Return(NEW_DEFAULT_HEIGHT));

    layout->set_size(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(layout->height(), NEW_DEFAULT_HEIGHT);
}

TEST(MainLayout, SetSize_NegativeWidth_ShouldThrowRunTimeError)
{
    auto [layout, stub_overlay] = construct();

    EXPECT_THROW(layout->set_size(-1, NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, SetSize_NegativeHeigth_ShouldThrowRunTimeError)
{
    auto [layout, stub_overlay] = construct();

    EXPECT_THROW(layout->set_size(NEW_DEFAULT_WIDTH, -1), std::runtime_error);
}

TEST(MainLayout, SetSize_WidthGreaterThan9999_ShouldThrowRunTimeError)
{
    auto [layout, stub_overlay] = construct();

    EXPECT_THROW(layout->set_size(10000, NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, SetSize_HeightGreaterThan9999_ShouldThrowRunTimeError)
{
    auto [layout, stub_overlay] = construct();

    EXPECT_THROW(layout->set_size(NEW_DEFAULT_WIDTH, 10000), std::runtime_error);
}

TEST(MainLayout, SetSize_WidthLesshanMinWidth160_ShouldThrowRunTimeError)
{
    auto [layout, stub_overlay] = construct();

    EXPECT_THROW(layout->set_size(159, DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, SetSize_HeightLessThanMinHeight120_ShouldThrowRunTimeError)
{
    auto [layout, stub_overlay] = construct();

    EXPECT_THROW(layout->set_size(DEFAULT_WIDTH, 119), std::runtime_error);
}

TEST(MainLayout, SetSize_VeryBigWidth_ShouldThrowRunTimeError)
{
    auto [layout, stub_overlay] = construct();

    EXPECT_THROW(layout->set_size(std::numeric_limits<int>::max(), NEW_DEFAULT_HEIGHT), std::runtime_error);
}

TEST(MainLayout, SetSize_VeryBigHeight_ShouldThrowRunTimeError)
{
    auto [layout, stub_overlay] = construct();

    EXPECT_THROW(layout->set_size(NEW_DEFAULT_WIDTH, std::numeric_limits<int>::max()), std::runtime_error);
}

TEST(MainLayout, SetSize_WithBackground_BackgroundSetSizeShouldBeCalled)
{
    auto [layout, stub_overlay] = construct();
    auto mock_background = create_background();
    layout->set_background(mock_background);

    EXPECT_CALL(*mock_background, set_size(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->set_size(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayout, SetSize_WithRegion_RegionSetSizeShouldBeCalled)
{
    auto [layout, stub_overlay] = construct();
    auto mock_region = add_region(*layout);

    ON_CALL(*mock_region, width()).WillByDefault(Return(NEW_DEFAULT_WIDTH));
    ON_CALL(*mock_region, height()).WillByDefault(Return(NEW_DEFAULT_HEIGHT));

    EXPECT_CALL(*mock_region, set_size(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->set_size(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayout, AddRegion_Null_HandlerAddChildShouldNotBeCalled)
{
    auto [layout, mock_overlay] = construct();

    EXPECT_CALL(*mock_overlay, add_child(_, _, _ ,_ , _)).Times(0);

    layout->add_region(nullptr);
}

TEST(MainLayout, AddRegion_Add1_HandlerAddChildShouldBeCalled)
{
    auto [layout, mock_overlay] = construct();

    EXPECT_CALL(*mock_overlay, add_child(_, _, _ ,_ , _));

    add_region(*layout);
}

TEST(MainLayout, Region_DefaultGet0Element_ShouldThrowOutOfRangeException)
{
    auto [layout, stub_overlay] = construct();

    ASSERT_THROW(layout->region(0), std::out_of_range);
}

TEST(MainLayout, Region_Add1RegionAndGet1_ShouldThrowOutOfRangeException)
{
    auto [layout, mock_overlay] = construct();

    add_region(*layout);

    ASSERT_THROW(layout->region(1), std::out_of_range);
}

TEST(MainLayout, Region_Add1RegionAndGetNegative_ShouldThrowOutOfRangeException)
{
    auto [layout, mock_overlay] = construct();

    add_region(*layout);

    ASSERT_THROW(layout->region(-10), std::out_of_range);
}

TEST(MainLayout, Region_Add1RegionAndGetBigIndex_ShouldThrowOutOfRangeException)
{
    auto [layout, mock_overlay] = construct();

    add_region(*layout);

    ASSERT_THROW(layout->region(std::numeric_limits<size_t>::max()), std::out_of_range);
}

TEST(MainLayout, Region_Add1RegionAndGet0_ShouldReturnAddedRegion)
{
    auto [layout, mock_overlay] = construct();

    auto stub_region = add_region(*layout);

    ASSERT_EQ(&layout->region(0), stub_region.get());
}

TEST(MainLayout, RegionsCount_Default_Equals0)
{
    auto [layout, stub_overlay] = construct();

    ASSERT_EQ(layout->regions_count(), 0);
}

TEST(MainLayout, RegionCount_Add3Regions_RegionCountEquals3)
{
    auto [layout, mock_overlay] = construct();
    const int REGIONS_COUNT = 3;

    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        add_region(*layout);
    }

    ASSERT_EQ(layout->regions_count(), REGIONS_COUNT);
}

TEST(MainLayout, Show_Default_HandlerShowShouldBeCalled)
{
    auto [layout, mock_overlay] = construct();

    EXPECT_CALL(*mock_overlay, show());

    layout->show();
}

TEST(MainLayout, Show_WithBackground_BackgroundShowShouldBeCalled)
{
    auto [layout, mock_overlay] = construct();
    auto mock_background = create_background();
    layout->set_background(mock_background);

    EXPECT_CALL(*mock_background, show());

    layout->show();
}

TEST(MainLayout, Show_WithRegion_RegionShowShouldBeCalled)
{
    auto [layout, mock_overlay] = construct();
    auto mock_region = add_region(*layout);

    EXPECT_CALL(*mock_region, show());

    layout->show();
}

TEST(MainLayout, Show_With3Regions_HandlerReorderChildShouldBeCalled)
{
    auto [layout, mock_overlay] = construct();
    const int REGIONS_COUNT = 3;

    for(int i = 0; i != REGIONS_COUNT; ++i)
    {
        auto stub_region = add_region(*layout);
    }

    EXPECT_CALL(*mock_overlay, reorder_child(_, _)).Times(REGIONS_COUNT);

    layout->show();
}
