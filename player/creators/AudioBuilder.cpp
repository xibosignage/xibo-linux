#include "AudioBuilder.hpp"
#include "media/AudioHandler.hpp"

#include <boost/optional/optional.hpp>

std::unique_ptr<Audio> AudioBuilder::create()
{
    return std::make_unique<Audio>(m_id, m_path, createHandler());
}

std::unique_ptr<IAudioHandler> AudioBuilder::createHandler()
{
    return std::make_unique<AudioHandler>();
}

void AudioBuilder::doSetup(Audio& audio)
{
    audio.setVolume(m_mute ? MIN_VOLUME : m_volume);
    audio.setLooped(m_loop);
}

AudioBuilder& AudioBuilder::mediaOptions(const ResourcesXlf::AudioOptions& opts)
{
    m_mute = getMuteOption(opts.muted());
    m_loop = getLoopOption(opts.looped());
    m_volume = getVolumeOption(opts.volume());
    return *this;
}

bool AudioBuilder::getMuteOption(const boost::optional<bool>& muteOpt)
{
    return muteOpt.value_or(DEFAULT_AUDIO_MUTED);
}

bool AudioBuilder::getLoopOption(const boost::optional<bool>& loopOpt)
{
   return loopOpt.value_or(DEFAULT_AUDIO_LOOPED);
}

int AudioBuilder::getVolumeOption(const boost::optional<int>& volumeOpt)
{
    return volumeOpt.value_or(MAX_VOLUME);
}
