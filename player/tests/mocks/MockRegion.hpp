#pragma once

#include "control/IRegion.hpp"
#include "control/IRegionContent.hpp"

#include <gmock/gmock.h>

class MockRegion : public IRegion
{
public:
    MockRegion(std::unique_ptr<IFixedLayoutAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    IFixedLayoutAdaptor& handler()
    {
        return *m_handler;
    }

    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD0(loopContent, void());
    MOCK_CONST_METHOD0(contentLooped, bool());
    MOCK_CONST_METHOD0(id, int());
    MOCK_CONST_METHOD0(zorder, int());
    MOCK_METHOD0(show, void());
    MOCK_METHOD3(addContent, void(std::unique_ptr<IRegionContent>&& content, int x, int y));
    MOCK_METHOD2(subscribe, void(EventType type, const EventHandler& handler));

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;

};
