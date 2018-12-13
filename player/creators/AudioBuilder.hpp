#include "MediaBuilder.hpp"

#include "media/IAudioHandler.hpp"
#include "media/Audio.hpp"

const bool DEFAULT_AUDIO_LOOPED = false;
const bool DEFAULT_AUDIO_MUTED = false;

class AudioBuilder : public BaseMediaBuilder<AudioBuilder>
{
public:
    std::unique_ptr<Audio> build();

    AudioBuilder& muted(const boost::optional<bool>& muted);
    AudioBuilder& looped(const boost::optional<bool>& looped);
    AudioBuilder& volume(const boost::optional<int>& volume);

protected:
    virtual std::unique_ptr<IAudioHandler> createHandler();

private:
    std::unique_ptr<Audio> createAudio();

private:
    int m_volume;
    bool m_looped;
    bool m_muted;

};
