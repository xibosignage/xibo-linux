#pragma once

#ifdef USE_GTK
#include "control/widgets/gtk/StatusScreenGtk.hpp"
#endif
#include "control/widgets/StatusScreen.hpp"

namespace StatusScreenFactory
{
    inline std::unique_ptr<Xibo::StatusScreen> create(int width, int height)
    {
#ifdef USE_GTK
        return std::make_unique<StatusScreenGtk>(width, height);
#else
        return nullptr;
#endif
    }
}
