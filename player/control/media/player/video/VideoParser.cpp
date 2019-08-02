#include "VideoParser.hpp"

#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/MediaResources.hpp"

const bool DefaultVideoMuted = false;
const bool DefaultVideoLooped = false;
const MediaGeometry::ScaleType DefaultVideoScaleType = MediaGeometry::ScaleType::Scaled;

ExtraOptions VideoParser::parseExtraOptionsImpl(const xml_node& node)
{
    auto muted = node.get<bool>(ResourcesXlf::Player::Mute, DefaultVideoMuted);
    auto looped = node.get<bool>(ResourcesXlf::Player::Loop, DefaultVideoLooped);

    return {
        {ResourcesXlf::Player::Mute, std::to_string(muted)},
        {ResourcesXlf::Player::Loop, std::to_string(looped)}
    };
}

MediaGeometry VideoParser::geometryFrom(const xml_node& node)
{
    auto scaleType = node.get<MediaGeometry::ScaleType>(ResourcesXlf::option(ResourcesXlf::Media::Geometry::ScaleType), DefaultVideoScaleType);

    return MediaGeometry{scaleType, MediaGeometry::Align::Center, MediaGeometry::Valign::Middle};
}
