#pragma once

#ifdef USE_GTK
#include "control/widgets/gtk/ImageGtk.hpp"
#endif
#include "control/widgets/Image.hpp"

namespace ImageWidgetFactory
{
    inline std::unique_ptr<Xibo::Image> create(int width, int height, bool useAlpha = false)
    {
#ifdef USE_GTK
        return std::make_unique<ImageGtk>(width, height, useAlpha);
#else
        return nullptr;
#endif
    }
}
