#pragma once

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

inline auto constructWindow()
{
    auto [window, adaptor] = construct<MainWindow, MockWindowAdaptor>();
    window->setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    return std::pair{window, adaptor};
}

inline auto constructWindowWithoutSize()
{
    auto [window, adaptor] = construct<MainWindow, MockWindowAdaptor>();
    return std::pair{window, adaptor};
}

inline auto constructMockLayout()
{
    auto handler = new testing::NiceMock<MockOverlayAdaptor>;
    auto layout = new testing::NiceMock<MockMainLayout>(unique(handler));

    ON_CALL(*layout, handler()).WillByDefault(testing::ReturnRef(*handler));

    return std::pair{layout, handler};
}

const int DEFAULT_WINDOW_POS = 100;

const auto invalidMainWindowSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, MIN_DISPLAY_HEIGHT>;

class MainWindowTestSize : public testing::TestWithParam<Size> { };
