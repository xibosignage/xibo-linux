#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "Media.hpp"

struct Video : Media
{
    Video(int id, int duration, Render render, const std::string& uri, bool mute, bool loop);

    bool mute;
    bool loop;
};

#endif // VIDEO_HPP
