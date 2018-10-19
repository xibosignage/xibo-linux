#include "MainWindowTests.hpp"

using namespace ::testing;

TEST_P(MainWindowTestSize, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    auto [window, windowHandlerStub] = constructWindowWithoutSize();

    ASSERT_THROW(window->setSize(GetParam().width, GetParam().height), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MainWindowTestSize, ::testing::ValuesIn(invalidMainWindowSizes));

TEST(MainWindowTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [window, windowHandlerStub] = constructWindow();

    ASSERT_EQ(&window->handler(), windowHandlerStub);
}

TEST(MainWindowTest, Constuctor_Default_HandlerDisableWindowDecorationShouldBeCalled)
{
    auto windowHandlerMock = std::make_unique<NiceMock<MockWindowAdaptor>>();

    EXPECT_CALL(*windowHandlerMock, disableWindowDecoration());

    std::make_shared<MainWindow>(std::move(windowHandlerMock));
}

TEST(MainWindowTest, Constuctor_Default_HandlerDisableWindowResizeShouldBeCalled)
{
    auto windowHandlerMock = std::make_unique<NiceMock<MockWindowAdaptor>>();

    EXPECT_CALL(*windowHandlerMock, disableWindowResize());

    std::make_shared<MainWindow>(std::move(windowHandlerMock));
}

TEST(MainWindowTest, SetKeepAbove_True_HandlerSetKeepAboveWithTrueShouldBeCalled)
{
    auto [window, windowHandlerMock] = constructWindow();

    EXPECT_CALL(*windowHandlerMock, setKeepAbove(true));

    window->setKeepAbove(true);
}

TEST(MainWindowTest, SetKeepAbove_False_HandlerSetKeepAboveWithFalseShouldBeCalled)
{
    auto [window, windowHandlerMock] = constructWindow();

    EXPECT_CALL(*windowHandlerMock, setKeepAbove(false));

    window->setKeepAbove(false);
}

TEST(MainWindowTest, SetCursorVisible_True_HandlerSetCursorVisibleWithTrueShouldBeCalled)
{
    auto [window, windowHandlerMock] = constructWindow();

    EXPECT_CALL(*windowHandlerMock, setCursorVisible(true));

    window->setCursorVisible(true);
}

TEST(MainWindowTest, SetCursorVisible_False_HandlerSetCursorVisibleWithFalseShouldBeCalled)
{
    auto [window, windowHandlerMock] = constructWindow();

    EXPECT_CALL(*windowHandlerMock, setCursorVisible(false));

    window->setCursorVisible(false);
}

TEST(MainWindowTest, SetFullscreen_True_HandlerFullscreenShouldBeCalled)
{
    auto [window, windowHandlerMock] = constructWindow();

    EXPECT_CALL(*windowHandlerMock, fullscreen());

    window->setFullscreen(true);
}

TEST(MainWindowTest, SetFullscreen_False_HandlerUnfullscreenShouldBeCalled)
{
    auto [window, windowHandlerMock] = constructWindow();

    EXPECT_CALL(*windowHandlerMock, unfullscreen());

    window->setFullscreen(false);
}

TEST(MainWindowTest, SetPos_XPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto [window, windowHandlerStub] = constructWindow();

    ASSERT_THROW(window->setPos(-1, DEFAULT_WINDOW_POS), std::invalid_argument);
}

TEST(MainWindowTest, SetPos_YPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto [window, windowHandlerStub] = constructWindow();

    ASSERT_THROW(window->setPos(DEFAULT_WINDOW_POS, -1), std::invalid_argument);
}

TEST(MainWindowTest, SetPos_BothPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto [window, windowHandlerStub] = constructWindow();

    ASSERT_THROW(window->setPos(-1, -1), std::invalid_argument);
}

TEST(MainWindowTest, SetPos_ValidPos_HandlerMoveWithValidPosShouldBeCalled)
{
    auto [window, windowHandlerMock] = constructWindow();

    EXPECT_CALL(*windowHandlerMock, move(DEFAULT_WINDOW_POS, DEFAULT_WINDOW_POS));

    window->setPos(DEFAULT_WINDOW_POS, DEFAULT_WINDOW_POS);
}

TEST(MainWindowTest, AddLayout_ValidLayout_AdaptorAddShouldBeCalled)
{
    auto [window, windowHandlerMock] = constructWindow();
    auto [stubLayout, layoutHandlerStub] = constructMockLayout();

    EXPECT_CALL(*windowHandlerMock, add(Ref(*layoutHandlerStub)));

    window->addLayout(unique(stubLayout));
}

TEST(MainWindowTest, IsVisible_HandlerIsVisibleTrue_WindowIsVisibleShouldReturnTrue)
{
    auto [window, windowHandlerStub] = constructWindow();

    ON_CALL(*windowHandlerStub, isShown()).WillByDefault(Return(true));

    ASSERT_EQ(window->isVisible(), true);
}

TEST(MainWindowTest, IsVisible_HandlerIsVisibleFalse_WindowIsVisibleShouldReturnFalse)
{
    auto [window, windowHandlerStub] = constructWindow();

    ON_CALL(*windowHandlerStub, isShown()).WillByDefault(Return(false));

    ASSERT_EQ(window->isVisible(), false);
}

TEST(MainWindowTest, ShowLayout_WithLayout_LayoutShowShouldNotBeCalled)
{
    auto [window, windowHandlerStub] = constructWindow();
    auto [mockLayout, layoutHandlerStub] = constructMockLayout();

    EXPECT_CALL(*mockLayout, show());

    window->addLayout(unique(mockLayout));
    window->showLayout();
}

TEST(MainWindowTest, ShowLayout_NoLayout_LayoutShowShouldNotBeCalled)
{
    auto [window, windowHandlerStub] = constructWindow();
    auto [mockLayoutRaw, layoutHandlerStub] = constructMockLayout();
    auto mockLayout = unique(mockLayoutRaw);

    EXPECT_CALL(*mockLayout, show()).Times(0);

    window->showLayout();
}
