#include "Audio.hpp"

#include "media/MediaVisitor.hpp"
#include "utils/Utilities.hpp"
#include "constants.hpp"

namespace ph = std::placeholders;

Audio::Audio(bool looped, std::unique_ptr<IAudioHandler>&& handler) :
    m_handler(std::move(handler)), m_looped(looped)
{
    m_handler->connect(std::bind(&Audio::onAudioFinished, this));
}

void Audio::doStart()
{
    m_handler->play();
}

void Audio::onAudioFinished()
{
    Utils::logger()->debug("Playback finished");

    if(m_looped)
    {
        Utils::logger()->debug("Looping enabled. Restarting...");
        m_handler->play();
    }
}

void Audio::doStop()
{
    m_handler->stop();
}

void Audio::apply(MediaVisitor& visitor)
{
    visitor.visit(*this);
}

