#include "VideoParser.hpp"

#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/creators/MediaResources.hpp"

const bool DEFAULT_VIDEO_MUTED = false;
const bool DEFAULT_VIDEO_LOOPED = false;
const MediaGeometry::ScaleType DEFAULT_VIDEO_SCALE_TYPE = MediaGeometry::ScaleType::Scaled;

ExtraOptions VideoParser::parseAdditonalOptions(const xml_node& node)
{
    auto muted = node.get<bool>(ResourcesXlf::option(ResourcesXlf::Player::Mute), DEFAULT_VIDEO_MUTED);
    auto looped = node.get<bool>(ResourcesXlf::option(ResourcesXlf::Player::Loop), DEFAULT_VIDEO_LOOPED);

    return {
        {ResourcesXlf::Player::Mute, std::to_string(muted)},
        {ResourcesXlf::Player::Loop, std::to_string(looped)}
    };
}

MediaGeometry VideoParser::geometry()
{
    auto scaleType = node().get<MediaGeometry::ScaleType>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::ScaleType), DEFAULT_VIDEO_SCALE_TYPE);

    return MediaGeometry{scaleType, MediaGeometry::Align::Center, MediaGeometry::Valign::Middle};
}
