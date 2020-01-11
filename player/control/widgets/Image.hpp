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

        virtual void fillColor(const Color& color) = 0;
        virtual void loadFrom(const Uri& uri, PreserveRatio preserveRatio) = 0;
    };
}

namespace ImageWidgetFactory
{
    std::unique_ptr<Xibo::Image> create(const Uri& uri,
                                        int width,
                                        int height,
                                        Xibo::Image::PreserveRatio preserveRatio);
    std::unique_ptr<Xibo::Image> create(const Color& color, int width, int height);
}
