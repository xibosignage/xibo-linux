#pragma once

#include "adaptors/IWidgetAdaptor.hpp"

#include <gmock/gmock.h>

class MockWidgetAdaptor : public IWidgetAdaptor
{
public:
    MOCK_METHOD1(apply, void(AdaptorVisitor& visitor));
};
