#include "AudioBuilder.hpp"

#include "media/Audio.hpp"
#include "media/AudioHandler.hpp"

const bool DEFAULT_LOOPED = false;
const bool DEFAULT_MUTED = false;

std::unique_ptr<IMedia> AudioBuilder::doBuild()
{
    auto audio = createAudio();
    audio->setVolume(m_muted ? MIN_VOLUME : m_volume);
    audio->setLooped(m_looped);
    return audio;
}

std::unique_ptr<Audio> AudioBuilder::createAudio()
{
    return std::make_unique<Audio>(m_path, createHandler());
}

std::unique_ptr<IAudioHandler> AudioBuilder::createHandler()
{
    return std::make_unique<AudioHandler>();
}

AudioBuilder& AudioBuilder::muted(const boost::optional<bool>& muted)
{
    m_muted = muted.value_or(DEFAULT_MUTED);
    return *this;
}

AudioBuilder& AudioBuilder::looped(const boost::optional<bool>& looped)
{
    m_looped = looped.value_or(DEFAULT_LOOPED);
    return *this;
}

AudioBuilder& AudioBuilder::volume(const boost::optional<int>& volume)
{
    m_volume = volume.value_or(MAX_VOLUME);
    return *this;
}
