#pragma once

#include "utils/ITimerProvider.hpp"
#include <gmock/gmock.h>

class MockTimerProvider : public ITimerProvider
{
public:
    MOCK_METHOD2(start, void(unsigned int msecs, std::function<bool()> handler));
    MOCK_METHOD2(startSeconds, void(unsigned int secs, std::function<bool()> handler));
    MOCK_METHOD2(startOnce, void(unsigned int msecs, std::function<void()> handler));
    MOCK_METHOD2(startOnceSeconds, void(unsigned int secs, std::function<void()> handler));
    MOCK_METHOD0(stop, void());
    MOCK_CONST_METHOD0(active, bool());

};
