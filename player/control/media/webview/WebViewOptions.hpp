#pragma once

#include "control/media/MediaOptions.hpp"

struct WebViewOptions : public MediaOptions
{
    enum Transparency
    {
        Disable,
        Enable
    };

    Transparency transparency;
    int m_mode;
};
