#include "MediaBuilder.hpp"

class IAudioHandler;
class Audio;

const bool DEFAULT_AUDIO_LOOPED = false;
const bool DEFAULT_AUDIO_MUTED = false;

class AudioBuilder : public MediaBuilder
{
public:
    AudioBuilder& muted(const boost::optional<bool>& muted);
    AudioBuilder& looped(const boost::optional<bool>& looped);
    AudioBuilder& volume(const boost::optional<int>& volume);

protected:
    std::unique_ptr<IMedia> doBuild() final;
    virtual std::unique_ptr<IAudioHandler> createHandler();

private:
    std::unique_ptr<Audio> createAudio();

private:
    int m_volume;
    bool m_looped;
    bool m_muted;

};
