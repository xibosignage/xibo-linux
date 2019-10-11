#pragma once

#include "control/widgets/Widget.hpp"

class Uri;

namespace Xibo
{
    class WebView : public Widget
    {
    public:
        enum Transparency
        {
            Disable,
            Enable
        };

        virtual void reload() = 0;
        virtual void load(const Uri& uri) = 0;
        virtual void enableTransparency() = 0;
    };
}
