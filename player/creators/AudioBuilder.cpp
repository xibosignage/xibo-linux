#include "AudioBuilder.hpp"

#include "media/AudioHandler.hpp"

#include <boost/optional/optional.hpp>

std::unique_ptr<Audio> AudioBuilder::create()
{
    return std::unique_ptr<Audio>(new Audio{m_id, m_path, createHandler()});
}

std::unique_ptr<IAudioHandler> AudioBuilder::createDefaultHandler()
{
    return std::make_unique<AudioHandler>();
}

void AudioBuilder::doMediaSetup(Audio& audio)
{
    audio.setVolume(m_mute == AudioOptions::Mute::Enable ? MIN_VOLUME : m_volume);
    audio.setLooped(m_loop);
}

AudioBuilder& AudioBuilder::retrieveMediaOptions(const AudioOptions& opts)
{
    m_mute = getMuteOption(opts.muted());
    m_loop = getLoopOption(opts.looped());
    m_volume = getVolumeOption(opts.volume());
    return *this;
}

AudioOptions::Mute AudioBuilder::getMuteOption(const boost::optional<AudioOptions::Mute>& muteOpt)
{
    return muteOpt.value_or(DEFAULT_AUDIO_MUTED);
}

AudioOptions::Loop AudioBuilder::getLoopOption(const boost::optional<AudioOptions::Loop>& loopOpt)
{
   return loopOpt.value_or(DEFAULT_AUDIO_LOOPED);
}

int AudioBuilder::getVolumeOption(const boost::optional<int>& volumeOpt)
{
    return volumeOpt.value_or(MAX_VOLUME);
}
