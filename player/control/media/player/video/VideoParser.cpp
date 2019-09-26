#include "VideoParser.hpp"

#include "control/media/MediaResources.hpp"
#include "control/media/player/MediaPlayerResources.hpp"

const bool DefaultVideoMuted = false;
const bool DefaultVideoLooped = false;
const MediaGeometry::ScaleType DefaultVideoScaleType = MediaGeometry::ScaleType::Scaled;

ExtraOptions VideoParser::extraOptionsImpl(const ptree_node& node)
{
    auto muted = node.get<bool>(XlfResources::Player::Mute, DefaultVideoMuted);
    auto looped = node.get<bool>(XlfResources::Player::Loop, DefaultVideoLooped);

    return {{XlfResources::Player::Mute, std::to_string(muted)}, {XlfResources::Player::Loop, std::to_string(looped)}};
}

MediaGeometry VideoParser::geometryFrom(const ptree_node& node)
{
    auto scaleType = node.get<MediaGeometry::ScaleType>(XlfResources::option(XlfResources::Media::Geometry::ScaleType),
                                                        DefaultVideoScaleType);

    return MediaGeometry{scaleType, MediaGeometry::Align::Center, MediaGeometry::Valign::Middle};
}
