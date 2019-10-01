#pragma once

#ifdef USE_GTK
#include "control/widgets/render/OutputWindowGtk.hpp"
#endif
#include "control/widgets/render/OutputWindow.hpp"

namespace OutputWindowFactory
{
    inline std::unique_ptr<Xibo::OutputWindow> create(int width, int height)
    {
#ifdef USE_GTK
        return std::make_unique<OutputWindowGtk>(width, height);
#else
        return nullptr;
#endif
    }
}
