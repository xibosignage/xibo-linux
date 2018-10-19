#include "AudioFactory.hpp"
#include "media/Audio.hpp"
#include "media/IMedia.hpp"
#include "utils/Resources.hpp"

AudioFactory::AudioFactory(const xlf_node& parentNode, const xlf_node& mediaNode) :
    MediaFactory(parentNode, mediaNode)
{
}

std::unique_ptr<IMedia> AudioFactory::doCreate()
{
//    int id = attrs().template get<int>("id");
    auto uri = Resources::directory() / options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");

    bool mute = options().get<bool>("mute", false);
    bool loop = options().get<bool>("loop", false);
    double volume = options().get<int>("volume", MAX_VOLUME) / static_cast<double>(MAX_VOLUME);

    return std::make_unique<Audio>(duration, uri.string(), mute, loop, volume);
}
