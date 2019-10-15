#pragma once

#ifdef USE_GTK
#include "screenshot/X11ScreenShoter.hpp"
#endif
#include "screenshot/ScreenShoter.hpp"

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
