#include "VideoParser.hpp"
#include "media/Video.hpp"
#include "utils/utilities.hpp"

VideoParser::VideoParser(const xlf_node& parentNode, const xlf_node& mediaNode) :
    MediaParser(parentNode, mediaNode)
{
}

std::unique_ptr<Media> VideoParser::doParse()
{
    int id = attrs().template get<int>("id");
    auto uri = utils::resourcesDir() / options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");
    int width = parentNode().get_child("<xmlattr>").get<double>("width");
    int height = parentNode().get_child("<xmlattr>").get<double>("height");

    bool mute = options().get<bool>("mute", false);
    bool loop = options().get<bool>("loop", false);

    return std::make_unique<Video>(id, width, height, duration, uri.string(), mute, loop);
}
