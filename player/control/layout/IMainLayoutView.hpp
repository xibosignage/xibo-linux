#pragma once

#include "control/common/IWidget.hpp"

#include <memory>

class IMainLayoutView : public IWidget
{
public:
    virtual void addRegion(const std::shared_ptr<IWidget>& child, int x, int y, int z) = 0;
    virtual void reorderRegion(const std::shared_ptr<IWidget>& child, int z) = 0;
    virtual void addBackground(const std::shared_ptr<IWidget>& mainChild) = 0;

};
