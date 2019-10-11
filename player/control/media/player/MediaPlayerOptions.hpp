#pragma once

#include "control/media/MediaOptions.hpp"

struct MediaPlayerOptions : MediaOptions
{
    enum class Mute
    {
        Disable,
        Enable
    };

    enum class Loop
    {
        Disable,
        Enable
    };

    enum Location
    {
        Local,
        Network
    };

    Mute muted;
    Loop looped;
    int volume;
};
