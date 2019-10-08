#pragma once

#ifdef USE_GTK
#include "control/widgets/gtk/OverlayLayoutGtk.hpp"
#endif
#include "control/widgets/OverlayLayout.hpp"

namespace OverlayLayoutFactory
{
    inline std::unique_ptr<Xibo::OverlayLayout> create(int width, int height)
    {
#ifdef USE_GTK
        return std::make_unique<OverlayLayoutGtk>(width, height);
#else
        return nullptr;
#endif
    }
}
