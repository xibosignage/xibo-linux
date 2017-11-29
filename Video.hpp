#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "Media.hpp"

struct Video : Media
{
    void InitOptions(std::map<std::string, std::string> options) override;

    struct Options : Media::BaseOptions
    {
        bool loop;
        bool mute;
    } options;
};

#endif // VIDEO_HPP
