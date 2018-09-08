#pragma once

#include "AdaptorVisitor.hpp"

class IWidgetAdaptor
{
public:
    virtual ~IWidgetAdaptor() = default;
    virtual void apply(AdaptorVisitor& visitor) = 0;

};
