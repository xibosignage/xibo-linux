#include "AudioParser.hpp"
#include "media/Audio.hpp"
#include "utils/utilities.hpp"

AudioParser::AudioParser(const xlf_node& parent_node, const xlf_node& media_node) :
    MediaParser(parent_node, media_node)
{
}

std::unique_ptr<Media> AudioParser::doParse()
{
    int id = attrs().template get<int>("id");
    std::string uri = utils::example_dir() + "/" + options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");

    bool mute = options().get<bool>("mute", false);
    bool loop = options().get<bool>("loop", false);
    double volume = options().get<int>("volume", MAX_VOLUME) / static_cast<double>(MAX_VOLUME);

    return std::make_unique<Audio>(id, duration, uri, mute, loop, volume);
}
