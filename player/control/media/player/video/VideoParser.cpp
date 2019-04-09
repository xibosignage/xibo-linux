#include "VideoParser.hpp"

#include "control/media/player/MediaPlayerResources.hpp"

const bool DEFAULT_VIDEO_MUTED = false;
const bool DEFAULT_VIDEO_LOOPED = false;

VideoParser::VideoParser(const xml_node& node) :
    MediaParser(node)
{
}

MediaPlayerOptions VideoParser::parse()
{
    auto options = baseOptions();
    auto muted = node().get<bool>(ResourcesXlf::option(ResourcesXlf::Player::Mute), DEFAULT_VIDEO_MUTED);
    auto looped = node().get<bool>(ResourcesXlf::option(ResourcesXlf::Player::Loop), DEFAULT_VIDEO_LOOPED);

    return MediaPlayerOptions{options,
                              static_cast<MediaPlayerOptions::Mute>(muted),
                              static_cast<MediaPlayerOptions::Loop>(looped),
                              MAX_VOLUME};
}
