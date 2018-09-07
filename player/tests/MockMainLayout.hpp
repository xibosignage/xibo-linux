#pragma once

#include "control/IMainLayout.hpp"

#include <gmock/gmock.h>

class MockMainLayout : public IMainLayout
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_METHOD1(setBackground, void(std::unique_ptr<IBackground> background));
    MOCK_METHOD0(background, IBackground&());
    MOCK_METHOD3(addMediaContainer, void(std::unique_ptr<IMediaContainer> mediaContainer, int x, int y));
    MOCK_METHOD0(removeAllContainers, void());
    MOCK_METHOD1(mediaContainer, IMediaContainer&(size_t index));
    MOCK_CONST_METHOD0(mediaContainersCount, size_t());
    MOCK_METHOD0(show, void());
    MOCK_METHOD0(handler, IOverlayAdaptor&());
};
