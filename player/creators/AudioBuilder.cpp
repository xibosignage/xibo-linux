#include "AudioBuilder.hpp"

#include "media/Audio.hpp"
#include "media/AudioHandler.hpp"

std::unique_ptr<IMedia> AudioBuilder::doBuild()
{
    auto audio = std::make_unique<Audio>(m_path, std::make_unique<AudioHandler>());
    audio->setVolume(m_volume);
    audio->setLooped(m_looped);
    return audio;
}

AudioBuilder& AudioBuilder::muted(const boost::optional<bool>&  muted)
{
    m_muted = muted.value_or(false);
    return *this;
}

AudioBuilder& AudioBuilder::looped(const boost::optional<bool>&  looped)
{
    m_looped = looped.value_or(false);
    return *this;
}

AudioBuilder& AudioBuilder::volume(const boost::optional<int>& volume)
{
    m_volume = volume.value_or(MAX_VOLUME);
    return *this;
}
