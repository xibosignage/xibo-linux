#include "Audio.hpp"
#include "Region.hpp"

const double MIN_VOLUME = 0.0;
const double MAX_VOLUME = 1.0;

Audio::Audio(const Region& region, int id, int duration, const std::string& uri, bool muted, bool looped) :
    Media(region, id, duration, Render::Native, uri), m_muted(muted), m_looped(looped)
{
//    m_handler->set_volume(m_muted ? MIN_VOLUME : MAX_VOLUME);
}

void Audio::stop()
{
    //m_handler->stop();
    // stop video
    Media::stop();
}

void Audio::start()
{
    //m_handler->play();
    Media::start();
}
