#include "VideoFactory.hpp"

#include "media/Video.hpp"
#include "media/IMedia.hpp"
#include "adaptors/VideoHandler.hpp"
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

    bool mute = options().get<bool>("mute", false);
    bool loop = options().get<bool>("loop", false);

    auto handler = std::make_unique<VideoHandler>();
    handler->setSize(width, height);
    handler->setVolume(mute ? 0 : 1.0);
    handler->load(uri);

    auto video = std::make_unique<Video>(loop, std::move(handler));
    video->setDuration(duration);
    return video;
}
