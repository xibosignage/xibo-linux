#pragma once

#include "control/widgets/Widget.hpp"

#include <memory>

namespace Xibo
{
    class OverlayLayout : public Widget
    {
    public:
        virtual void addChild(const std::shared_ptr<Widget>& child, int x, int y, int z) = 0;
        virtual void removeChildren() = 0;
        virtual void reorderChild(const std::shared_ptr<Widget>& child, int z) = 0;
        virtual void setMainChild(const std::shared_ptr<Widget>& mainChild) = 0;
    };
}
