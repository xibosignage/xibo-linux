#pragma once

#include "BaseTestWithHandler.hpp"

#include "control/MainWindow.hpp"
#include "mocks/MockWindowAdaptor.hpp"

#include "mocks/MockMainLayout.hpp"
#include "mocks/MockOverlayAdaptor.hpp"

const int DEFAULT_WINDOW_POS = 100;
const auto invalidMainWindowSizes = invalidSizes<MIN_DISPLAY_WIDTH, MIN_DISPLAY_HEIGHT>;

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
