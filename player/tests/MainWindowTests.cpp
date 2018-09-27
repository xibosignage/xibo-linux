#include "test_utils.hpp"

#include "control/MainWindow.hpp"

#include "control/IBackground.hpp"
#include "control/IMediaContainer.hpp"
#include "adaptors/IOverlayAdaptor.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"
#include "adaptors/IImageAdaptor.hpp"

#include "mocks/MockWindowAdaptor.hpp"
#include "mocks/MockMainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"

using namespace ::testing;

auto construct_window()
{
    auto [window, adaptor] = construct<MainWindow, MockWindowAdaptor>();
    window->setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    return std::pair{window, adaptor};
}

auto construct_window_without_size()
{
    auto [window, adaptor] = construct<MainWindow, MockWindowAdaptor>();
    return std::pair{window, adaptor};
}

auto construct_mock_layout()
{
    auto stubLayout = new NiceMock<MockMainLayout>;
    auto layoutHandlerStub = std::make_shared<NiceMock<MockOverlayAdaptor>>();

    ON_CALL(*stubLayout, handler()).WillByDefault(ReturnRef(*layoutHandlerStub));

    return std::pair{stubLayout, layoutHandlerStub};
}

const int DEFAULT_POS = 100;

const auto invalidMainWindowSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, MIN_DISPLAY_HEIGHT>;

class MainWindowTestSize : public TestWithParam<Size> { };

TEST_P(MainWindowTestSize, SetSize_InvalidSize_ShouldThrowInvalidArgError)
{
    auto [window, windowHandlerStub] = construct_window_without_size();

    ASSERT_THROW(window->setSize(GetParam().width, GetParam().height), std::invalid_argument);
}

INSTANTIATE_TEST_CASE_P(Suite, MainWindowTestSize, ::testing::ValuesIn(invalidMainWindowSizes));

TEST(MainWindowTest, Handler_Default_EqualsToPreviouslyPassedAdaptor)
{
    auto [window, windowHandlerStub] = construct_window();

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
    auto [window, windowHandlerMock] = construct_window();

    EXPECT_CALL(*windowHandlerMock, setKeepAbove(true));

    window->setKeepAbove(true);
}

TEST(MainWindowTest, SetKeepAbove_False_HandlerSetKeepAboveWithFalseShouldBeCalled)
{
    auto [window, windowHandlerMock] = construct_window();

    EXPECT_CALL(*windowHandlerMock, setKeepAbove(false));

    window->setKeepAbove(false);
}

TEST(MainWindowTest, SetCursorVisible_True_HandlerSetCursorVisibleWithTrueShouldBeCalled)
{
    auto [window, windowHandlerMock] = construct_window();

    EXPECT_CALL(*windowHandlerMock, setCursorVisible(true));

    window->setCursorVisible(true);
}

TEST(MainWindowTest, SetCursorVisible_False_HandlerSetCursorVisibleWithFalseShouldBeCalled)
{
    auto [window, windowHandlerMock] = construct_window();

    EXPECT_CALL(*windowHandlerMock, setCursorVisible(false));

    window->setCursorVisible(false);
}

TEST(MainWindowTest, SetFullscreen_True_HandlerFullscreenShouldBeCalled)
{
    auto [window, windowHandlerMock] = construct_window();

    EXPECT_CALL(*windowHandlerMock, fullscreen());

    window->setFullscreen(true);
}

TEST(MainWindowTest, SetFullscreen_False_HandlerUnfullscreenShouldBeCalled)
{
    auto [window, windowHandlerMock] = construct_window();

    EXPECT_CALL(*windowHandlerMock, unfullscreen());

    window->setFullscreen(false);
}

TEST(MainWindowTest, SetPos_XPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto [window, windowHandlerStub] = construct_window();

    ASSERT_THROW(window->setPos(-1, DEFAULT_POS), std::invalid_argument);
}

TEST(MainWindowTest, SetPos_YPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto [window, windowHandlerStub] = construct_window();

    ASSERT_THROW(window->setPos(DEFAULT_POS, -1), std::invalid_argument);
}

TEST(MainWindowTest, SetPos_BothPosNegative_InvalidArgErrorShouldBeThrown)
{
    auto [window, windowHandlerStub] = construct_window();

    ASSERT_THROW(window->setPos(-1, -1), std::invalid_argument);
}

TEST(MainWindowTest, SetPos_ValidPos_HandlerMoveWithValidPosShouldBeCalled)
{
    auto [window, windowHandlerMock] = construct_window();

    EXPECT_CALL(*windowHandlerMock, move(DEFAULT_POS, DEFAULT_POS));

    window->setPos(DEFAULT_POS, DEFAULT_POS);
}

TEST(MainWindowTest, AddLayout_ValidLayout_AdaptorAddShouldBeCalled)
{
    auto [window, windowHandlerMock] = construct_window();
    auto [stubLayout, layoutHandlerStub] = construct_mock_layout();

    EXPECT_CALL(*windowHandlerMock, add(Ref(*layoutHandlerStub)));

    window->addLayout(unique(stubLayout));
}

TEST(MainWindowTest, IsVisible_HandlerIsVisibleTrue_WindowIsVisibleShouldReturnTrue)
{
    auto [window, windowHandlerStub] = construct_window();

    ON_CALL(*windowHandlerStub, isVisible()).WillByDefault(Return(true));

    ASSERT_EQ(window->isVisible(), true);
}

TEST(MainWindowTest, IsVisible_HandlerIsVisibleFalse_WindowIsVisibleShouldReturnFalse)
{
    auto [window, windowHandlerStub] = construct_window();

    ON_CALL(*windowHandlerStub, isVisible()).WillByDefault(Return(false));

    ASSERT_EQ(window->isVisible(), false);
}

TEST(MainWindowTest, ShowLayout_WithLayout_LayoutShowShouldNotBeCalled)
{
    auto [window, windowHandlerStub] = construct_window();
    auto [mockLayout, layoutHandlerStub] = construct_mock_layout();

    EXPECT_CALL(*mockLayout, show());

    window->addLayout(unique(mockLayout));
    window->showLayout();
}

TEST(MainWindowTest, ShowLayout_NoLayout_LayoutShowShouldNotBeCalled)
{
    auto [window, windowHandlerStub] = construct_window();
    auto [mockLayoutRaw, layoutHandlerStub] = construct_mock_layout();
    auto mockLayout = unique(mockLayoutRaw);

    EXPECT_CALL(*mockLayout, show()).Times(0);

    window->showLayout();
}
