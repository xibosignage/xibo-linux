#pragma once

#include "control/common/IWidget.hpp"

#include <memory>

class IOverlayLayout : public IWidget
{
public:
    virtual void addChild(const std::shared_ptr<IWidget>& child, int x, int y, int z) = 0;
    virtual void removeChildren() = 0;
    virtual void reorderChild(const std::shared_ptr<IWidget>& child, int z) = 0;
    virtual void setMainChild(const std::shared_ptr<IWidget>& mainChild) = 0;
};
