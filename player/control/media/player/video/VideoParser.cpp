#include "VideoParser.hpp"

#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/MediaResources.hpp"

const bool DEFAULT_VIDEO_MUTED = false;
const bool DEFAULT_VIDEO_LOOPED = false;
const MediaGeometry::ScaleType DEFAULT_VIDEO_SCALE_TYPE = MediaGeometry::ScaleType::Scaled;

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

MediaGeometry VideoParser::geometry()
{
    auto scaleType = node().get<MediaGeometry::ScaleType>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::ScaleType), DEFAULT_VIDEO_SCALE_TYPE);

    return MediaGeometry{scaleType, MediaGeometry::Align::Center, MediaGeometry::Valign::Middle};
}
