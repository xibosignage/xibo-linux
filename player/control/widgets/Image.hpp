#pragma once

#include "common/types/Color.hpp"
#include "control/widgets/Widget.hpp"

#include <cstdint>

class Uri;

namespace Xibo
{
    class Image : public Widget
    {
    public:
        enum class PreserveRatio
        {
            False,
            True
        };

        virtual void setColor(const Color& color) = 0;
        virtual void loadFrom(const Uri& uri, PreserveRatio preserveRatio) = 0;
    };

    using ImagePtr = std::shared_ptr<Image>;
}
