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

const int DEFAULT_WINDOW_POS = 100;

const auto invalidMainWindowSizes = invalidSizes<MAX_DISPLAY_WIDTH, MIN_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, MIN_DISPLAY_HEIGHT>;

inline auto constructMockLayout()
{
    auto handler = new testing::NiceMock<MockOverlayAdaptor>;
    auto layout = new testing::NiceMock<MockMainLayout>(unique(handler));

    ON_CALL(*layout, handler()).WillByDefault(testing::ReturnRef(*handler));

    return layout;
}

class MainWindowTest : public testing::Test
{
public:
    auto constructWindow()
    {
        auto window = construct<MainWindow>(unique(m_adaptor));
        addLayout(window);
        return window;
    }

protected:
    void SetUp() override
    {
        m_adaptor = new testing::NiceMock<MockWindowAdaptor>;
    }

    void TearDown() override
    {
        m_layout = nullptr;
        m_adaptor = nullptr;
    }

    MockWindowAdaptor& adaptor()
    {
        return *m_adaptor;
    }

    MockMainLayout& layout()
    {
        return *m_layout;
    }

private:
    void addLayout(std::shared_ptr<MainWindow> window)
    {
        m_layout = constructMockLayout();

        window->addLayout(unique(m_layout));
    }

private:
    MockWindowAdaptor* m_adaptor = nullptr;
    MockMainLayout* m_layout = nullptr;

};

class MainWindowTestSize : public MainWindowTest, public testing::WithParamInterface<Size> { };
