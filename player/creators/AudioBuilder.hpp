#include "MediaBuilder.hpp"

#include "media/AudioHandler.hpp"
#include "media/Audio.hpp"
#include "parsers/AudioOptions.hpp"

const bool DEFAULT_AUDIO_LOOPED = false;
const bool DEFAULT_AUDIO_MUTED = false;

class AudioBuilder;

template<>
struct BuilderTraits<AudioBuilder>
{
    using Component = Audio;
    using DefaultHandler = AudioHandler;
    using Options = ResourcesXlf::AudioOptions;
};

class AudioBuilder : public AbstractMediaBuilder<AudioBuilder>
{
protected:
    AudioBuilder& retrieveMediaOptions(const ResourcesXlf::AudioOptions& opts) override;
    std::unique_ptr<Audio> create() override;
    void doMediaSetup(Audio& audio) override;

private:
    bool getMuteOption(const boost::optional<bool>& muteOpt);
    bool getLoopOption(const boost::optional<bool>& loopOpt);
    int getVolumeOption(const boost::optional<int>& volumeOpt);

private:
    int m_volume;
    bool m_mute;
    bool m_loop;

};
