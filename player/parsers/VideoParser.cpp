#include "VideoParser.hpp"
#include "media/Video.hpp"
#include "utils/utilities.hpp"

VideoParser::VideoParser(const xlf_node& parent_node, const xlf_node& media_node) :
    MediaParser(parent_node, media_node)
{
}

std::unique_ptr<Media> VideoParser::doParse()
{
    int id = attrs().template get<int>("id");
    auto uri = utils::resources_dir() / options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");
    int width = parent_node().get_child("<xmlattr>").get<double>("width");
    int height = parent_node().get_child("<xmlattr>").get<double>("height");

    bool mute = options().get<bool>("mute", false);
    bool loop = options().get<bool>("loop", false);

    return std::make_unique<Video>(id, Size{width, height}, duration, uri.string(), mute, loop);
}
