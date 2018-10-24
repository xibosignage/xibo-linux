#include "VideoFactory.hpp"

#include "media/Video.hpp"
#include "media/IMedia.hpp"
#include "media/VideoHandler.hpp"

#include "utils/Resources.hpp"

VideoFactory::VideoFactory(const xlf_node& parentNode, const xlf_node& mediaNode) :
    MediaFactory(parentNode, mediaNode)
{
}

std::unique_ptr<IMedia> VideoFactory::doCreate()
{
    auto uri = Resources::directory() / options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");
    int width = static_cast<int>(parentNode().get_child("<xmlattr>").get<double>("width"));
    int height = static_cast<int>(parentNode().get_child("<xmlattr>").get<double>("height"));

    bool muted = options().get<bool>("mute", false);
    bool looped = options().get<bool>("loop", false);

    auto video = std::make_unique<Video>(width, height, uri, std::make_unique<VideoHandler>());
    video->setMuted(muted);
    video->setLooped(looped);
    video->setDuration(duration);
    return video;
}
