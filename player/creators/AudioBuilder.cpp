#include "AudioBuilder.hpp"
#include "media/AudioHandler.hpp"

#include <boost/optional/optional.hpp>

std::unique_ptr<Audio> AudioBuilder::build()
{
    auto audio = createAudio();
    prepareCommonParams(*audio);
    audio->setVolume(m_muted ? MIN_VOLUME : m_volume);
    audio->setLooped(m_looped);
    return audio;
}

std::unique_ptr<Audio> AudioBuilder::createAudio()
{
    return std::make_unique<Audio>(m_id, m_path, createHandler());
}

std::unique_ptr<IAudioHandler> AudioBuilder::createHandler()
{
    return std::make_unique<AudioHandler>();
}

AudioBuilder& AudioBuilder::muted(const boost::optional<bool>& muted)
{
    m_muted = muted.value_or(DEFAULT_AUDIO_MUTED);
    return *this;
}

AudioBuilder& AudioBuilder::looped(const boost::optional<bool>& looped)
{
    m_looped = looped.value_or(DEFAULT_AUDIO_LOOPED);
    return *this;
}

AudioBuilder& AudioBuilder::volume(const boost::optional<int>& volume)
{
    m_volume = volume.value_or(MAX_VOLUME);
    return *this;
}
