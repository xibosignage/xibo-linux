#include "MainWindowTests.hpp"

using namespace ::testing;

TEST_F(MainWindowTest, Constuctor_Default_HandlerDisableWindowDecorationShouldBeCalled)
{
    EXPECT_CALL(adaptor(), disableWindowDecoration());

    constructWindow();
}

TEST_F(MainWindowTest, Constuctor_Default_HandlerDisableWindowResizeShouldBeCalled)
{
    EXPECT_CALL(adaptor(), disableWindowResize());

    constructWindow();
}

TEST_F(MainWindowTest, SetSize_Default_HandlerSetSizeShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    window->setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

TEST_P(MainWindowTestSize, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    auto window = constructWindow();

    ASSERT_THROW(window->setSize(GetParam().width, GetParam().height), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MainWindowTestSize, ::testing::ValuesIn(invalidMainWindowSizes));

TEST_F(MainWindowTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto window = constructWindow();

    ASSERT_EQ(&window->handler(), &adaptor());
}

TEST_F(MainWindowTest, SetKeepAbove_True_HandlerSetKeepAboveWithTrueShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), setKeepAbove(true));

    window->setKeepAbove(true);
}

TEST_F(MainWindowTest, SetKeepAbove_False_HandlerSetKeepAboveWithFalseShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), setKeepAbove(false));

    window->setKeepAbove(false);
}

TEST_F(MainWindowTest, SetCursorVisible_True_HandlerSetCursorVisibleWithTrueShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), setCursorVisible(true));

    window->setCursorVisible(true);
}

TEST_F(MainWindowTest, SetCursorVisible_False_HandlerSetCursorVisibleWithFalseShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), setCursorVisible(false));

    window->setCursorVisible(false);
}

TEST_F(MainWindowTest, SetFullscreen_True_HandlerFullscreenShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), fullscreen());

    window->setFullscreen(true);
}

TEST_F(MainWindowTest, SetFullscreen_False_HandlerUnfullscreenShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), unfullscreen());

    window->setFullscreen(false);
}

TEST_F(MainWindowTest, SetPos_XPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto window = constructWindow();

    ASSERT_THROW(window->setPos(-1, DEFAULT_WINDOW_POS), std::invalid_argument);
}

TEST_F(MainWindowTest, SetPos_YPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto window = constructWindow();

    ASSERT_THROW(window->setPos(DEFAULT_WINDOW_POS, -1), std::invalid_argument);
}

TEST_F(MainWindowTest, SetPos_BothPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto window = constructWindow();

    ASSERT_THROW(window->setPos(-1, -1), std::invalid_argument);
}

TEST_F(MainWindowTest, SetPos_ValidPos_HandlerMoveWithValidPosShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), move(DEFAULT_WINDOW_POS, DEFAULT_WINDOW_POS));

    window->setPos(DEFAULT_WINDOW_POS, DEFAULT_WINDOW_POS);
}

TEST_P(MainWindowTestScale, AddLayout_ValidLayout_AdaptorScaleShouldBeCalled)
{
    auto window = constructWindow();
    auto layout = constructLayout();

    Size playerResolution = GetParam().playerResolution;
    Size windowSize = GetParam().windowSize;
    double scaleFactor = GetParam().scaleFactor;

    ON_CALL(*layout, width()).WillByDefault(Return(playerResolution.width));
    ON_CALL(*layout, height()).WillByDefault(Return(playerResolution.height));
    ON_CALL(adaptor(), width()).WillByDefault(Return(windowSize.width));
    ON_CALL(adaptor(), height()).WillByDefault(Return(windowSize.height));

    EXPECT_CALL(*layout, scale(scaleFactor, scaleFactor));

    window->setLayout(std::move(layout));
}

INSTANTIATE_TEST_CASE_P(Suite, MainWindowTestScale, ::testing::ValuesIn(scalesForResolutions));

TEST_F(MainWindowTest, AddLayout_ValidLayout_AdaptorAddShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), add(_));

    window->setLayout(constructLayout());
}

TEST_F(MainWindowTest, IsVisible_HandlerIsVisibleTrue_WindowIsVisibleShouldReturnTrue)
{
    auto window = constructWindow();

    ON_CALL(adaptor(), isShown()).WillByDefault(Return(true));

    ASSERT_EQ(window->isVisible(), true);
}

TEST_F(MainWindowTest, IsVisible_HandlerIsVisibleFalse_WindowIsVisibleShouldReturnFalse)
{
    auto window = constructWindow();

    ON_CALL(adaptor(), isShown()).WillByDefault(Return(false));

    ASSERT_EQ(window->isVisible(), false);
}

TEST_F(MainWindowTest, ShowLayout_WithLayout_LayoutShowShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(layout(), show());

    window->showLayout();
}

TEST_F(MainWindowTest, Show_Default_HandlerShowShouldBeCalled)
{
    auto window = constructWindow();

    EXPECT_CALL(adaptor(), show());

    window->show();
}
