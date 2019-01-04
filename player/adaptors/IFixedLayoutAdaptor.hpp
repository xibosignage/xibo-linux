#pragma once

#include "IWidgetAdaptor.hpp"

class IFixedLayoutAdaptor : public IWidgetAdaptor
{
public:
    virtual ~IFixedLayoutAdaptor() = default;
    virtual void addChild(IWidgetAdaptor& child, int left, int top) = 0;
    virtual void removeChildren() = 0;
};
