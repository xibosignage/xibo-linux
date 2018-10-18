#include "MainLayoutTests.hpp"

using namespace ::testing;

NiceMock<MockBackground>* createBackground()
{
    auto handler = new NiceMock<MockImageAdaptor>;
    auto background = new NiceMock<MockBackground>(unique(handler));

    ON_CALL(*background, handler()).WillByDefault(ReturnRef(*handler));

    return background;
}

NiceMock<MockMediaContainer>* createMediaContainer()
{
    auto handler = new NiceMock<MockFixedLayoutAdaptor>;
    auto mediaContainer = new NiceMock<MockMediaContainer>(unique(handler));

    ON_CALL(*mediaContainer, handler()).WillByDefault(ReturnRef(*handler));

    return mediaContainer;
}

auto constructLayout()
{
    auto [layout, handler] = construct<MainLayout, MockOverlayAdaptor>();
    layout->setBackground(unique(createBackground()));
    layout->addMediaContainer(unique(createMediaContainer()), DEFAULT_XPOS, DEFAULT_XPOS);
    return std::pair{layout, handler};
}

auto constructLayoutWithoutBackground()
{
    auto [layout, handler] = construct<MainLayout, MockOverlayAdaptor>();
    layout->addMediaContainer(unique(createMediaContainer()), DEFAULT_XPOS, DEFAULT_XPOS);
    return std::pair{layout, handler};
}

auto constructLayoutWithoutContainer()
{
    auto [layout, handler] = construct<MainLayout, MockOverlayAdaptor>();
    layout->setBackground(unique(createBackground()));
    return std::pair{layout, handler};
}

TEST(MainLayoutTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [layout, layoutHandlerStub] = constructLayout();

    ASSERT_EQ(&layout->handler(), layoutHandlerStub);
}

TEST(MainLayoutTest, Width_HandlerReturnsDefaultWidth_LayoutWidthEqualsDefault)
{
    auto [layout, layoutHandlerStub] = constructLayout();

    ON_CALL(*layoutHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH));

    ASSERT_EQ(layout->width(), DEFAULT_WIDTH);
}

TEST(MainLayoutTest, Height_HandlerReturnsDefaultHeight_LayoutHeightEqualsDefault)
{
    auto [layout, layoutHandlerStub] = constructLayout();

    ON_CALL(*layoutHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT));

    ASSERT_EQ(layout->height(), DEFAULT_HEIGHT);
}

TEST(MainLayoutTest, SetBackground_Valid_HandlerAddMainChildShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = constructLayoutWithoutBackground();

    EXPECT_CALL(*layoutHandlerMock, addMainChild(_));

    layout->setBackground(unique(createBackground()));
}

TEST(MainLayoutTest, SetBackground_BackgroundWidthNotEqualLayoutWidth_InvalidArgErrorShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutBackground();
    auto stubBackground = createBackground();

    ON_CALL(*layoutHandlerStub, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->setBackground(unique(stubBackground)), std::invalid_argument);
}

TEST(MainLayoutTest, SetBackground_BackgroundHeightNotEqualLayoutHeight_InvalidArgErrorShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutBackground();
    auto stubBackground = createBackground();

    ON_CALL(*layoutHandlerStub, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->setBackground(unique(stubBackground)), std::invalid_argument);
}

TEST(MainLayoutTest, SetBackground_2InRow_HandlerAddMainChild2TimesCalledRemoveMainChild1TimeCalled)
{
    auto [layout, layoutHandlerMock] = constructLayoutWithoutBackground();

    EXPECT_CALL(*layoutHandlerMock, addMainChild(_)).Times(2);
    EXPECT_CALL(*layoutHandlerMock, removeMainChild()).Times(1);

    layout->setBackground(unique(createBackground()));
    layout->setBackground(unique(createBackground()));
}

TEST(MainLayoutTest, AddMediaContainer_Add1_HandlerAddChildShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = constructLayoutWithoutContainer();

    EXPECT_CALL(*layoutHandlerMock, addChild(_, _, _ ,_ , _));

    layout->addMediaContainer(unique(createMediaContainer()), DEFAULT_XPOS, DEFAULT_YPOS);
}

TEST_P(MainLayoutTestPos, AddMediaContainer_InvalidPos_ShouldThrowInvalidArgError)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutContainer();
    auto stubContainer = createMediaContainer();

    ASSERT_THROW(layout->addMediaContainer(unique(stubContainer), GetParam().x, GetParam().y), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutTestPos, ::testing::ValuesIn(invalidMainLayoutPos));

TEST(MainLayoutTest, AddMediaContainer_ContainerWidthGreaterThanLayoutWidth_InvalidArgErrorShouldBeThrown)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutContainer();
    auto stubContainer = createMediaContainer();

    ON_CALL(*stubContainer, width()).WillByDefault(Return(DEFAULT_WIDTH + 1));

    ASSERT_THROW(layout->addMediaContainer(unique(stubContainer), DEFAULT_XPOS, DEFAULT_YPOS), std::invalid_argument);
}

TEST(MainLayoutTest, AddMediaContainer_ContainerHeightGreaterThanLayoutWidth_InvalidArgErrorShouldBeThrown)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutContainer();
    auto stubContainer = createMediaContainer();

    ON_CALL(*stubContainer, height()).WillByDefault(Return(DEFAULT_HEIGHT + 1));

    ASSERT_THROW(layout->addMediaContainer(unique(stubContainer), DEFAULT_XPOS, DEFAULT_YPOS), std::invalid_argument);
}

TEST(MainLayoutTest, Scale_Default_HandlerScaleShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = constructLayout();

    EXPECT_CALL(*layoutHandlerMock, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    layout->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST(MainLayoutTest, Scale_Default_ContainerScaleShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = constructLayoutWithoutContainer();
    auto mockContainer = createMediaContainer();
    layout->addMediaContainer(unique(mockContainer), DEFAULT_XPOS, DEFAULT_YPOS);

    EXPECT_CALL(*mockContainer, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    layout->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST(MainLayoutTest, Scale_Default_BackgroundScaleShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = constructLayoutWithoutBackground();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, scale(DEFAULT_XSCALE, DEFAULT_YSCALE));

    layout->scale(DEFAULT_XSCALE, DEFAULT_YSCALE);
}

TEST(MainLayoutTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto [layout, layoutHandlerMock] = constructLayout();

    EXPECT_CALL(*layoutHandlerMock, show());
    ON_CALL(*layoutHandlerMock, isShown()).WillByDefault(Return(false));

    layout->show();
}

TEST(MainLayoutTest, Show_CallTwice_HandlerShowShouldBeCalledOnce)
{
    auto [layout, layoutHandlerMock] = constructLayout();

    EXPECT_CALL(*layoutHandlerMock, show()).Times(1);

    layout->show();
    ON_CALL(*layoutHandlerMock, isShown()).WillByDefault(Return(true));
    layout->show();
}

TEST(MainLayoutTest, Show_WithBackground_BackgroundShowShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutBackground();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, show());
    ON_CALL(*layoutHandlerStub, isShown()).WillByDefault(Return(false));

    layout->show();
}

TEST(MainLayoutTest, Show_WithBackgroundTwice_BackgroundShowShouldBeCalledOnce)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutBackground();
    auto mockBackground = createBackground();
    layout->setBackground(unique(mockBackground));

    EXPECT_CALL(*mockBackground, show()).Times(1);

    layout->show();
    ON_CALL(*layoutHandlerStub, isShown()).WillByDefault(Return(true));
    layout->show();
}

TEST(MainLayoutTest, Show_WithMediaContainer_MediaContainerShowShouldBeCalled)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutContainer();
    auto mockContainer = createMediaContainer();
    layout->addMediaContainer(unique(mockContainer), DEFAULT_XPOS, DEFAULT_YPOS);

    EXPECT_CALL(*mockContainer, show());
    ON_CALL(*layoutHandlerStub, isShown()).WillByDefault(Return(false));

    layout->show();
}

TEST(MainLayoutTest, Show_WithMediaContainerTwice_MediaContainerShowShouldBeCalledOnce)
{
    auto [layout, layoutHandlerStub] = constructLayoutWithoutContainer();
    auto mockContainer = createMediaContainer();
    layout->addMediaContainer(unique(mockContainer), DEFAULT_XPOS, DEFAULT_YPOS);

    EXPECT_CALL(*mockContainer, show()).Times(1);

    layout->show();
    ON_CALL(*layoutHandlerStub, isShown()).WillByDefault(Return(true));
    layout->show();
}

TEST_P(MainLayoutReorderTest, Show_With3MediaContainers_HandlerReorderChildShouldBeCalledWithCorrectOrder)
{
    auto [layout, layoutHandlerMock] = constructLayoutWithoutContainer();

    for(int zorder : GetParam())
        addContainer(*layout, zorder);

    for(size_t index = 0; index != GetParam().size(); ++index)
        EXPECT_CALL(*layoutHandlerMock, reorderChild(Ref(container(index)->handler()), static_cast<int>(index)));

    layout->show();
}

INSTANTIATE_TEST_CASE_P(Suite, MainLayoutReorderTest, ::testing::ValuesIn(zorders));
