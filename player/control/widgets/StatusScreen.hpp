#pragma once

#include "control/widgets/Widget.hpp"

#include <string>

namespace Xibo
{
    class StatusScreen : public Widget
    {
    public:
        virtual void setText(const std::string& text) = 0;
    };
}
