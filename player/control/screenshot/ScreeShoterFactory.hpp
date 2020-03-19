#pragma once

#ifdef USE_GTK
#include "control/screenshot/gtk/X11ScreenShoter.hpp"
#endif
#include "control/screenshot/ScreenShoter.hpp"

namespace ScreenShoterFactory
{
    inline std::unique_ptr<ScreenShoter> create(Xibo::Window& window)
    {
#ifdef USE_GTK
        return std::make_unique<X11ScreenShoter>(window);
#else
        return nullptr;
#endif
    }
}
