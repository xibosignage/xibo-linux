#include "test_utils.hpp"

#include "control/MainLayout.hpp"

#include "mocks/MockOverlayAdaptor.hpp"
#include "mocks/MockBackground.hpp"
#include "mocks/MockImageAdaptor.hpp"
#include "mocks/MockMediaContainer.hpp"
#include "mocks/MockFixedLayoutAdaptor.hpp"

using namespace ::testing;

auto construct_layout()
{
    auto [layout, adaptor] = construct<MainLayout, MockOverlayAdaptor>();
    layout->setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    return std::pair{layout, adaptor};
}

auto construct_layout_without_size()
{
    auto [layout, adaptor] = construct<MainLayout, MockOverlayAdaptor>();
    return std::pair{layout, adaptor};
}

NiceMock<MockBackground>* createBackground()
{
    auto background = new NiceMock<MockBackground>;
    auto backgroundHandlerStub = std::make_unique<NiceMock<MockImageAdaptor>>();

    ON_CALL(*background, handler()).WillByDefault(ReturnRef(*backgroundHandlerStub));

    return background;
}

NiceMock<MockMediaContainer>* createMediaContainer()
{
    auto mediaContainer = new NiceMock<MockMediaContainer>;
    auto containerHandlerStub = std::make_unique<NiceMock<MockFixedLayoutAdaptor>>();

    ON_CALL(*mediaContainer, handler()).WillByDefault(ReturnRef(*containerHandlerStub));

    return mediaContainer;
}

const auto invalidMainLayoutSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, MIN_DISPLAY_HEIGHT>;

class MainLayoutTest : public TestWithParam<Size> { };

TEST_P(MainLayoutTest, SetSize_InvalidSize_ShouldThrowRunTimeError)
{
    auto [layout, layoutHandlerStub] = construct_layout_without_size();

    ASSERT_THROW(layout->setSize(GetParam().width, GetParam().height), std::runtime_error);
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutTest, ::testing::ValuesIn(invalidMainLayoutSizes));

TEST(MainLayoutTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [layout, layoutHandlerStub] = construct_layout();

    ASSERT_EQ(&layout->handler(), layoutHandlerStub);
}

TEST(MainLayoutTest, Width_HandlerReturnsDefaultWidth_LayoutWidthEqualsDefault)
{
    auto [layout, layoutHandlerStub] = construct_layout();

    ON_CALL(*layoutHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
}

TEST(MainLayoutTest, Height_HandlerReturnsDefaultHeight_LayoutHeightEqualsDefault)
{
    auto [layout, layoutHandlerStub] = construct_layout();

    ON_CALL(*layoutHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, SetBackground_Null_HandlerAddMainChildShouldNotBeCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();

    EXPECT_CALL(*layoutHandlerMock, addMainChild(_)).Times(0);

    ASSERT_THROW(layout->setBackground(nullptr), std::runtime_error);
}

TEST(MainLayoutTest, SetBackground_NotNull_HandlerAddMainChildShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();

    EXPECT_CALL(*layoutHandlerMock, addMainChild(_));

    layout->setBackground(unique(createBackground()));
}

TEST(MainLayoutTest, SetBackground_BackgroundWidthNotEqualLayoutWidth_RunTimeErrorShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = construct_layout();
    auto stubBackground = createBackground();

    ON_CALL(*layoutHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->setBackground(unique(stubBackground)), std::runtime_error);
}

TEST(MainLayoutTest, SetBackground_BackgroundHeightNotEqualLayoutHeight_RunTimeErrorShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = construct_layout();
    auto stubBackground = createBackground();

    ON_CALL(*layoutHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->setBackground(unique(stubBackground)), std::runtime_error);
}

TEST(MainLayoutTest, SetBackground_2InRow_HandlerAddMainChild2TimesCalledRemoveMainChild1TimeCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();

    EXPECT_CALL(*layoutHandlerMock, addMainChild(_)).Times(2);
    EXPECT_CALL(*layoutHandlerMock, removeMainChild()).Times(1);

    layout->setBackground(unique(createBackground()));
    layout->setBackground(unique(createBackground()));
}

TEST(MainLayoutTest, SetSize_Width200Height200_HandlerSetSize200Width200HeightCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();

    EXPECT_CALL(*layoutHandlerMock, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, SetSize_Width200Height200_LayoutWidthEquals200)
{
    auto [layout, layoutHandlerStub] = construct_layout();

    ON_CALL(*layoutHandlerStub, width()).WillByDefault(Return(NEW_DEFAULT_WIDTH));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(layout->width(), NEW_DEFAULT_WIDTH);
}

TEST(MainLayoutTest, SetSize_Width200Height200_LayoutHeightEquals200)
{
    auto [layout, layoutHandlerStub] = construct_layout();

    ON_CALL(*layoutHandlerStub, height()).WillByDefault(Return(NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);

    ASSERT_EQ(layout->height(), NEW_DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, SetSize_WithBackground_BackgroundSetSizeShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = construct_layout();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT));

    layout->setSize(NEW_DEFAULT_WIDTH, NEW_DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, AddMediaContainer_NullContainer_HandlerAddChildShouldNotBeCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();

    EXPECT_CALL(*layoutHandlerMock, addChild(_, _, _, _ , _)).Times(0);

    ASSERT_THROW(layout->addMediaContainer(nullptr, DEFAULT_X_POS, DEFAULT_Y_POS), std::runtime_error);
}

TEST(MainLayoutTest, AddMediaContainer_Add1_HandlerAddChildShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();

    EXPECT_CALL(*layoutHandlerMock, addChild(_, _, _ ,_ , _));

    layout->addMediaContainer(unique(createMediaContainer()), DEFAULT_X_POS, DEFAULT_Y_POS);
}

TEST(MainLayoutTest, AddMediaContainer_ContainerWidthGreaterThanLayoutWidth_RunTimeErrorShouldBeThrown)
{
    auto [layout, layoutHandlerStub] = construct_layout();
    auto stubContainer = createMediaContainer();

    ON_CALL(*stubContainer, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->addMediaContainer(unique(stubContainer), DEFAULT_X_POS, DEFAULT_Y_POS), std::runtime_error);
}

TEST(MainLayoutTest, AddMediaContainer_ContainerHeightGreaterThanLayoutWidth_RunTimeErrorShouldBeThrown)
{
    auto [layout, layoutHandlerStub] = construct_layout();
    auto stubContainer = createMediaContainer();

    ON_CALL(*stubContainer, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->addMediaContainer(unique(stubContainer), DEFAULT_X_POS, DEFAULT_Y_POS), std::runtime_error);
}

TEST(MainLayoutTest, RemoveAllCotnainers_Default_HandlerRemoveChildrenShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();

    EXPECT_CALL(*layoutHandlerMock, removeChildren());

    layout->removeAllContainers();
}

TEST(MainLayoutTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();

    EXPECT_CALL(*layoutHandlerMock, show());

    layout->show();
}

TEST(MainLayoutTest, Show_WithBackground_BackgroundShowShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = construct_layout();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, show());

    layout->show();
}

TEST(MainLayoutTest, Show_WithMediaContainer_MediaContainerShowShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = construct_layout();
    auto mockContainer = createMediaContainer();
    layout->addMediaContainer(unique(mockContainer), DEFAULT_X_POS, DEFAULT_Y_POS);

    EXPECT_CALL(*mockContainer, show());

    layout->show();
}

// TEST zorder
TEST(MainLayoutTest, Show_With3MediaContainers_HandlerReorderChildShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = construct_layout();
    const int CONTAINERS_COUNT = 3;
    for(int i = 0; i != CONTAINERS_COUNT; ++i)
    {
        layout->addMediaContainer(unique(createMediaContainer()), DEFAULT_X_POS, DEFAULT_Y_POS);
    }

    EXPECT_CALL(*layoutHandlerMock, reorderChild(_, _)).Times(CONTAINERS_COUNT);

    layout->show();
}
