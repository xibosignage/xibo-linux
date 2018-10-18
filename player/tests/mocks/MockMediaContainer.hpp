#pragma once

#include "control/IMediaContainer.hpp"
#include "media/IMedia.hpp"

#include <gmock/gmock.h>

class MockMediaContainer : public IMediaContainer
{
public:
    MockMediaContainer(std::unique_ptr<IFixedLayoutAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_CONST_METHOD0(zorder, int());
    MOCK_METHOD0(show, void());
    MOCK_METHOD3(addMedia, void(std::unique_ptr<IMedia>&& media, int x, int y));
    MOCK_METHOD1(addMedia, void(std::unique_ptr<IMedia>&& media));
    MOCK_METHOD0(handler, IFixedLayoutAdaptor&());

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;

};
