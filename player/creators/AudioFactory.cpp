#include "AudioFactory.hpp"

#include "media/Audio.hpp"
#include "media/IMedia.hpp"
#include "media/AudioHandler.hpp"

#include "utils/Resources.hpp"

AudioFactory::AudioFactory(const xlf_node& parentNode, const xlf_node& mediaNode) :
    MediaFactory(parentNode, mediaNode)
{
}

std::unique_ptr<IMedia> AudioFactory::doCreate()
{
    auto path = Resources::directory() / options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");

    bool mute = options().get<bool>("mute", false);
    bool looped = options().get<bool>("loop", false);
    int volume = mute ? MIN_VOLUME : options().get<int>("volume", MAX_VOLUME);

    auto audio = std::make_unique<Audio>(path, std::make_unique<AudioHandler>());
    audio->setVolume(volume);
    audio->setLooped(looped);
    audio->setDuration(duration);
    return audio;
}
