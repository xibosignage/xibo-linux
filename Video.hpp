#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "Media.hpp"

class Video : public Media
{
public:
    Video(int id, int duration, Render render, const std::string& uri, bool mute, bool loop);

    bool mute() const;
    bool loop() const;

private:
    bool m_mute;
    bool m_loop;
};

#endif // VIDEO_HPP
