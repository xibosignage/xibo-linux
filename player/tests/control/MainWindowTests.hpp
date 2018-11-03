#pragma once

#include "BaseTestWithHandler.hpp"

#include "control/MainWindow.hpp"
#include "mocks/MockWindowAdaptor.hpp"

#include "mocks/MockMainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"

const int DEFAULT_WINDOW_POS = 100;
const auto invalidMainWindowSizes = invalidSizes<MIN_DISPLAY_WIDTH, MIN_DISPLAY_HEIGHT>;

struct ScaleForResolution
{
    Size windowSize;
    Size playerResolution;
    double scaleFactor;
};
const Size DEFAULT_RESOLUTION = Size{1920, 1080};
const std::vector<ScaleForResolution> scalesForResolutions{{Size{960, 432}, DEFAULT_RESOLUTION, 0.4},
                                                           {Size{1920, 432}, DEFAULT_RESOLUTION, 0.4},
                                                           {Size{960, 1080}, DEFAULT_RESOLUTION, 0.5},
                                                           {Size{2400, 432}, DEFAULT_RESOLUTION, 0.4},
                                                           {Size{960, 1620}, DEFAULT_RESOLUTION, 0.5},
                                                           {Size{1920, 1080}, DEFAULT_RESOLUTION, 1},
                                                           {Size{2400, 1620}, DEFAULT_RESOLUTION, 1.25},
                                                           {Size{1920, 1620}, DEFAULT_RESOLUTION, 1},
                                                           {Size{2400, 1080}, DEFAULT_RESOLUTION, 1}};

class MainWindowTest : public BaseTestWithHandler<MockWindowAdaptor>
{
public:
    auto constructWindow()
    {
        auto window = construct<MainWindow>(unique(&adaptor()));
        addLayout(*window);
        return window;
    }

    auto constructLayout()
    {
        return constructMock<MockMainLayout, MockOverlayAdaptor>();
    }

    MockMainLayout& layout()
    {
        return *m_layout;
    }

private:
    void addLayout(MainWindow& window)
    {
        auto layout = constructLayout();
        m_layout = layout.get();

        window.addLayout(std::move(layout));
    }

private:
    MockMainLayout* m_layout = nullptr;

};

class MainWindowTestSize : public MainWindowTest, public testing::WithParamInterface<Size> { };
class MainWindowTestScale : public MainWindowTest, public testing::WithParamInterface<ScaleForResolution> { };
