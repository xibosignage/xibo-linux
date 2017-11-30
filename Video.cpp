#include "Video.hpp"

Video::Video(int id, int duration, Render render, const std::string& uri, bool mute, bool loop) :
    Media(id, duration, render, uri), mute(mute), loop(loop)
{

}
