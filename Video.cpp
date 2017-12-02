#include "Video.hpp"

Video::Video(int id, int duration, Render render, const std::string& uri, bool mute, bool loop) :
    Media(id, duration, render, uri), m_mute(mute), m_loop(loop)
{

}

bool Video::mute() const
{
    return m_mute;
}

bool Video::loop() const
{
    return m_loop;
}
