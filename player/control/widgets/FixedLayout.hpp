#pragma once

#include "control/widgets/Widget.hpp"

#include <memory>

namespace Xibo
{
    class FixedLayout : public Widget
    {
    public:
        virtual void addChild(const std::shared_ptr<Widget>& child, int x, int y) = 0;
    };
}
