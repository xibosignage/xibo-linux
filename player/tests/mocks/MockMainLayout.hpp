#pragma once

#include "control/IMainLayout.hpp"

#include <gmock/gmock.h>

class MockMainLayout : public IMainLayout
{
public:
    MockMainLayout(std::unique_ptr<IOverlayAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    IOverlayAdaptor& handler()
    {
        return *m_handler;
    }

    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD1(setBackground, void(std::unique_ptr<IBackground>&& background));
    MOCK_METHOD3(addRegion, void(std::unique_ptr<IRegion>&& region, int x, int y));
    MOCK_METHOD0(show, void());
    MOCK_METHOD1(handleEvent, void(const Event& ev));
    MOCK_METHOD2(subscribe, void(EventType type, const EventHandler& handler));
    MOCK_METHOD1(pushEvent, void(const Event& ev));

private:
    std::unique_ptr<IOverlayAdaptor> m_handler;

};
