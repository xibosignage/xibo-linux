#include "test_utils.hpp"

#include "control/MainWindow.hpp"

#include "control/IBackground.hpp"
#include "control/IRegion.hpp"
#include "adaptors/IOverlayAdaptor.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"
#include "adaptors/IImageAdaptor.hpp"

#include "MockWindowAdaptor.hpp"
#include "MockMainLayout.hpp"
#include "MockOverlayAdaptor.hpp"

using namespace ::testing;

auto construct_window(int width, int height)
{
    return construct<MainWindow, MockWindowAdaptor>(width, height);
}

auto construct_window()
{
    return construct<MainWindow, MockWindowAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

auto construct_mock_layout()
{
    auto stubLayout = new NiceMock<MockMainLayout>;
    auto stubOverlay = std::make_shared<NiceMock<MockOverlayAdaptor>>();

    ON_CALL(*stubLayout, handler()).WillByDefault(ReturnRef(*stubOverlay));

    return std::pair{stubLayout, stubOverlay};
}

const int DEFAULT_POS = 100;

class MainWindowTest : public TestWithParam<Size> { };

TEST_P(MainWindowTest, Constructor_InvalidSize_ShouldThrowRunTimeError)
{
    ASSERT_THROW(construct_window(GetParam().width, GetParam().height), std::runtime_error);
}

INSTANTIATE_TEST_CASE_P(Suite, MainWindowTest, ::testing::ValuesIn(invalidSizes));

TEST(MainWindowTest, Constuctor_Default_HandlerSetDefaultSizeShouldBeCalled)
{
    auto mockWindowAdaptor = std::make_unique<NiceMock<MockWindowAdaptor>>();

    EXPECT_CALL(*mockWindowAdaptor, setDefaultSize(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    std::make_shared<MainWindow>(DEFAULT_WIDTH, DEFAULT_HEIGHT, std::move(mockWindowAdaptor));
}

TEST(MainWindowTest, Constuctor_Default_HandlerDisableWindowDecorationShouldBeCalled)
{
    auto mockWindowAdaptor = std::make_unique<NiceMock<MockWindowAdaptor>>();

    EXPECT_CALL(*mockWindowAdaptor, disableWindowDecoration());

    std::make_shared<MainWindow>(DEFAULT_WIDTH, DEFAULT_HEIGHT, std::move(mockWindowAdaptor));
}

TEST(MainWindowTest, Constuctor_Default_HandlerDisableWindowResizeShouldBeCalled)
{
    auto mockWindowAdaptor = std::make_unique<NiceMock<MockWindowAdaptor>>();

    EXPECT_CALL(*mockWindowAdaptor, disableWindowResize());

    std::make_shared<MainWindow>(DEFAULT_WIDTH, DEFAULT_HEIGHT, std::move(mockWindowAdaptor));
}

TEST(MainWindowTest, SetKeepAbove_True_HandlerSetKeepAboveWithTrueShouldBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();

    EXPECT_CALL(*mockWindowAdaptor, setKeepAbove(true));

    window->setKeepAbove(true);
}

TEST(MainWindowTest, SetKeepAbove_False_HandlerSetKeepAboveWithFalseShouldBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();

    EXPECT_CALL(*mockWindowAdaptor, setKeepAbove(false));

    window->setKeepAbove(false);
}

TEST(MainWindowTest, SetCursorVisible_True_HandlerSetCursorVisibleWithTrueShouldBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();

    EXPECT_CALL(*mockWindowAdaptor, setCursorVisible(true));

    window->setCursorVisible(true);
}

TEST(MainWindowTest, SetCursorVisible_False_HandlerSetCursorVisibleWithFalseShouldBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();

    EXPECT_CALL(*mockWindowAdaptor, setCursorVisible(false));

    window->setCursorVisible(false);
}

TEST(MainWindowTest, SetFullscreen_True_HandlerFullscreenShouldBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();

    EXPECT_CALL(*mockWindowAdaptor, fullscreen());

    window->setFullscreen(true);
}

TEST(MainWindowTest, SetFullscreen_False_HandlerUnfullscreenShouldBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();

    EXPECT_CALL(*mockWindowAdaptor, unfullscreen());

    window->setFullscreen(false);
}

TEST(MainWindowTest, SetPos_XPosNegative_RunTimeErrorShouldBeThrown)
{
    auto [window, stubWindowAdaptor] = construct_window();

    ASSERT_THROW(window->setPos(-1, DEFAULT_POS), std::runtime_error);
}

TEST(MainWindowTest, SetPos_YPosNegative_RunTimeErrorShouldBeThrown)
{
    auto [window, stubWindowAdaptor] = construct_window();

    ASSERT_THROW(window->setPos(DEFAULT_POS, -1), std::runtime_error);
}

TEST(MainWindowTest, SetPos_BothPosNegative_RunTimeErrorShouldBeThrown)
{
    auto [window, stubWindowAdaptor] = construct_window();

    ASSERT_THROW(window->setPos(-1, -1), std::runtime_error);
}

TEST(MainWindowTest, SetPos_ValidPos_HandlerMoveWithValidPosShouldBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();

    EXPECT_CALL(*mockWindowAdaptor, move(DEFAULT_POS, DEFAULT_POS));

    window->setPos(DEFAULT_POS, DEFAULT_POS);
}

TEST(MainWindowTest, AddLayout_NullLayout_LayoutShowShouldNotBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();

    EXPECT_CALL(*mockWindowAdaptor, add(_)).Times(0);

    ASSERT_THROW(window->addLayout(nullptr), std::runtime_error);
}

TEST(MainWindowTest, ShowLayout_ValidLayout_LayoutShowShouldNotBeCalled)
{
    auto [window, mockWindowAdaptor] = construct_window();
    auto [stubLayout, stubOverlay] = construct_mock_layout();

    EXPECT_CALL(*mockWindowAdaptor, add(Ref(*stubOverlay)));

    window->addLayout(unique(stubLayout));
}

TEST(MainWindowTest, IsVisible_HandlerIsVisibleTrue_WindowShouldReturnTrue)
{
    auto [window, stubWindowAdaptor] = construct_window();

    ON_CALL(*stubWindowAdaptor, isVisible()).WillByDefault(Return(true));

    ASSERT_EQ(window->isVisible(), true);
}

TEST(MainWindowTest, IsVisible_HandlerIsVisibleFalse_WindowShouldReturnFalse)
{
    auto [window, stubWindowAdaptor] = construct_window();

    ON_CALL(*stubWindowAdaptor, isVisible()).WillByDefault(Return(false));

    ASSERT_EQ(window->isVisible(), false);
}

TEST(MainWindowTest, ShowLayout_WithLayout_LayoutShowShouldNotBeCalled)
{
    auto [window, stubWindowAdaptor] = construct_window();
    auto [mockLayout, stubOverlay] = construct_mock_layout();

    EXPECT_CALL(*mockLayout, show());

    window->addLayout(unique(mockLayout));
    window->showLayout();
}
