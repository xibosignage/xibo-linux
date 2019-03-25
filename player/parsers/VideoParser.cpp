#include "VideoParser.hpp"
#include "XlfResources.hpp"

const bool DEFAULT_VIDEO_MUTED = false;
const bool DEFAULT_VIDEO_LOOPED = false;

VideoParser::VideoParser(const xml_node& node) :
    MediaParser(node)
{
}

VideoOptions VideoParser::parse()
{
    auto options = baseOptions();
    auto muted = node().get<bool>(ResourcesXlf::option(ResourcesXlf::Media::Video::Mute), DEFAULT_VIDEO_MUTED);
    auto looped = node().get<bool>(ResourcesXlf::option(ResourcesXlf::Media::Video::Loop), DEFAULT_VIDEO_LOOPED);

    return VideoOptions{options, muted, looped};
}
