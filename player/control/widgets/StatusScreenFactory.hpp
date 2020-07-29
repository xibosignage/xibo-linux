#pragma once

#ifdef USE_GTK
#include "control/widgets/gtk/StatusScreenGtk.hpp"
#endif
#include "control/widgets/StatusScreen.hpp"

namespace StatusScreenFactory
{
    template <typename Window>
    inline std::unique_ptr<Xibo::StatusScreen> create(Window& parentWindow, int width, int height)
    {
#ifdef USE_GTK
        return std::make_unique<StatusScreenGtk>(parentWindow, width, height);
#else
        return nullptr;
#endif
    }
}
