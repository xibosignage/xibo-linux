#pragma once

#include "control/common/IWidget.hpp"

#include <memory>

class IRegionView : public IWidget
{
public:
    virtual void addMedia(const std::shared_ptr<IWidget>& child, int left, int top) = 0;
};
