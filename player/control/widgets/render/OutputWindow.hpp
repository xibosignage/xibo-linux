#pragma once

#include "control/widgets/render/RenderFrame.hpp"
#include "control/widgets/Widget.hpp"

#include <memory>

namespace Xibo
{
    class OutputWindow : public Widget
    {
    public:
        virtual void drawFrame(const std::shared_ptr<RenderFrame>& frame) = 0;
    };
};
