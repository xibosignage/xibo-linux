#pragma once

#include "control/IRegion.hpp"

#include <gmock/gmock.h>

class MockRegion : public IRegion
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_CONST_METHOD0(left, int());
    MOCK_CONST_METHOD0(top, int());
    MOCK_METHOD2(setPos, void(int left, int top));
    MOCK_CONST_METHOD0(id, int());
    MOCK_CONST_METHOD0(zindex, int());
    MOCK_CONST_METHOD0(looped, bool());
    MOCK_METHOD0(show, void());
    MOCK_METHOD1(addMedia, void(std::unique_ptr<IMedia> media));
    MOCK_METHOD0(handler, IFixedLayoutAdaptor&());
};
