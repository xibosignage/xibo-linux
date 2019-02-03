#include "MediaBuilder.hpp"

#include "media/IAudioHandler.hpp"
#include "media/Audio.hpp"
#include "parsers/AudioOptions.hpp"

const bool DEFAULT_AUDIO_LOOPED = false;
const bool DEFAULT_AUDIO_MUTED = false;

class AudioBuilder : public BaseMediaBuilder<AudioBuilder, ResourcesXlf::AudioOptions>
{
public:
    std::unique_ptr<Audio> build();

protected:
    virtual std::unique_ptr<IAudioHandler> createHandler();
    AudioBuilder& mediaOptions(const ResourcesXlf::AudioOptions& opts) override;

private:
    std::unique_ptr<Audio> createAudio();

    bool getMuteOption(const boost::optional<bool>& muteOpt);
    bool getLoopOption(const boost::optional<bool>& loopOpt);
    int getVolumeOption(const boost::optional<int>& volumeOpt);

private:
    int m_volume;
    bool m_mute;
    bool m_loop;

};
