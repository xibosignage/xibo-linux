#pragma once

#include "utils/ITimerProvider.hpp"
#include <gmock/gmock.h>

class MockTimerProvider : public ITimerProvider
{
public:
    MOCK_METHOD2(start, void(unsigned int msec, std::function<void()> handler));
    MOCK_METHOD2(startOnce, void(unsigned int msec, std::function<void()> handler));
    MOCK_METHOD0(stop, void());

};
