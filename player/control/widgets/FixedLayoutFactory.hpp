#pragma once

#ifdef USE_GTK
#include "control/widgets/gtk/FixedLayoutGtk.hpp"
#endif
#include "control/widgets/FixedLayout.hpp"

namespace FixedLayoutFactory
{
    std::unique_ptr<Xibo::FixedLayout> create(int width, int height)
    {
#ifdef USE_GTK
        return std::make_unique<FixedLayoutGtk>(width, height);
#else
        return nullptr;
#endif
    }
}
