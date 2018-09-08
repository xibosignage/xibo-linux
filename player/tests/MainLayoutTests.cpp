#include "test_utils.hpp"

#include "control/MainLayout.hpp"

#include "MockOverlayAdaptor.hpp"
#include "MockBackground.hpp"
#include "MockImageAdaptor.hpp"
#include "MockMediaContainer.hpp"
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

NiceMock<MockMediaContainer>* createMediaContainer()
{
    auto mediaContainer = new NiceMock<MockMediaContainer>;
    auto stubFixed = std::make_unique<NiceMock<MockFixedLayoutAdaptor>>();

    ON_CALL(*mediaContainer, handler()).WillByDefault(ReturnRef(*stubFixed));

    return mediaContainer;
}

const auto invalidMainLayoutSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, MIN_DISPLAY_HEIGHT>;

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

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutTest, ::testing::ValuesIn(invalidMainLayoutSizes));

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

TEST(MainLayoutTest, AddMediaContainer_Null_HandlerAddChildShouldNotBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, addChild(_, _, _ ,_ , _)).Times(0);

    layout->addMediaContainer(nullptr, DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MainLayoutTest, AddMediaContainer_Add1_HandlerAddChildShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();

    EXPECT_CALL(*mockOverlay, addChild(_, _, _ ,_ , _));

    layout->addMediaContainer(unique(createMediaContainer()), DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MainLayoutTest, AddMediaContainer_ContainerWidthGreaterThanLayoutWidth_RunTimeErrorShouldBeThrown)
{
    auto [layout, stubOverlay] = construct_layout();
    auto stubContainer = createMediaContainer();

    ON_CALL(*stubContainer, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->addMediaContainer(unique(stubContainer), DEFAULT_X_POS, DEFAULT_Y_POS), std::runtime_error);
}

TEST(MainLayoutTest, AddMediaContainer_ContainerHeightGreaterThanLayoutWidth_RunTimeErrorShouldBeThrown)
{
    auto [layout, stubOverlay] = construct_layout();
    auto stubContainer = createMediaContainer();

    ON_CALL(*stubContainer, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->addMediaContainer(unique(stubContainer), DEFAULT_X_POS, DEFAULT_Y_POS), std::runtime_error);
}

TEST(MainLayoutTest, RemoveAllCotnainers_With3Containers_HandlerRemoveChildrenShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();
    const int CONTAINERS_COUNT = 3;
    for(int i = 0; i != CONTAINERS_COUNT; ++i)
    {
        layout->addMediaContainer(unique(createMediaContainer()), DEFAULT_X_POS, DEFAULT_Y_POS);
    }

    EXPECT_CALL(*mockOverlay, removeChildren());

    layout->removeAllContainers();
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

TEST(MainLayoutTest, Show_WithMediaContainer_MediaContainerShowShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();
    auto mockContainer = createMediaContainer();
    layout->addMediaContainer(unique(mockContainer), DEFAULT_X_POS, DEFAULT_Y_POS);

    EXPECT_CALL(*mockContainer, show());

    layout->show();
}

TEST(MainLayoutTest, Show_With3MediaContainers_HandlerReorderChildShouldBeCalled)
{
    auto [layout, mockOverlay] = construct_layout();
    const int CONTAINERS_COUNT = 3;
    for(int i = 0; i != CONTAINERS_COUNT; ++i)
    {
        layout->addMediaContainer(unique(createMediaContainer()), DEFAULT_X_POS, DEFAULT_Y_POS);
    }

    EXPECT_CALL(*mockOverlay, reorderChild(_, _)).Times(CONTAINERS_COUNT);

    layout->show();
}
