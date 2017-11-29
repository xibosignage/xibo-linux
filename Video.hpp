#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "Media.hpp"

struct Video : Media
{
    Video() = default;

    bool loop;
    bool mute;
};

#endif // VIDEO_HPP
