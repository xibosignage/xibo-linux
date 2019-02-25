#include "AbstractMediaBuilder.hpp"

#include "media/IAudioHandler.hpp"
#include "media/Audio.hpp"
#include "options/AudioOptions.hpp"

const AudioOptions::Mute DEFAULT_AUDIO_MUTED = AudioOptions::Mute::Disable;
const AudioOptions::Loop DEFAULT_AUDIO_LOOPED = AudioOptions::Loop::Disable;

class AudioBuilder;

template<>
struct BuilderTraits<AudioBuilder>
{
    using Component = Audio;
    using Handler = IAudioHandler;
    using Options = AudioOptions;
};

class AudioBuilder : public AbstractMediaBuilder<AudioBuilder>
{
protected:
    void retrieveMediaOptions(const AudioOptions& opts) override;
    std::unique_ptr<Audio> create() override;
    std::unique_ptr<IAudioHandler> createDefaultHandler() override;
    void doMediaSetup(Audio& audio) override;

private:
    AudioOptions::Mute getMuteOption(const boost::optional<AudioOptions::Mute>& muteOpt);
    AudioOptions::Loop getLoopOption(const boost::optional<AudioOptions::Loop>& loopOpt);
    int getVolumeOption(const boost::optional<int>& volumeOpt);

private:
    int m_volume;
    AudioOptions::Mute m_mute;
    AudioOptions::Loop m_loop;

};
