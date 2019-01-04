#include "Audio.hpp"

#include "media/MediaVisitor.hpp"
#include "utils/Utilities.hpp"
#include "constants.hpp"

namespace ph = std::placeholders;

Audio::Audio(int id, const FilePath& path, std::unique_ptr<IAudioHandler>&& handler) :
    Media(id), m_handler(std::move(handler))
{
    assert(m_handler);

    m_handler->load(path);
    m_handler->connect(std::bind(&Audio::onAudioFinished, this));
}

void Audio::doStart()
{
    m_handler->play();
}

void Audio::doStop()
{
    m_handler->stop();
}

void Audio::apply(MediaVisitor& visitor)
{
    visitor.visit(*this);
}

void Audio::stopPlayback()
{
    m_handler->stopPlayback();
}

void Audio::onAudioFinished()
{
    if(m_looped)
    {
        Utils::logger()->debug("Looping enabled. Restarting...");
        m_handler->play();
    }
}

void Audio::setVolume(int volume)
{
    m_handler->setVolume(volume);
}

void Audio::setLooped(bool looped)
{
    m_looped = looped;
}


