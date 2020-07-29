#include "VideoParser.hpp"

#include "common/constants.hpp"
#include "control/media/Media.hpp"
#include "control/media/MediaResources.hpp"
#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/player/video/VideoFactory.hpp"

const bool DefaultVideoMuted = false;
const bool DefaultVideoLooped = false;
const MediaGeometry::ScaleType DefaultVideoScaleType = MediaGeometry::ScaleType::Scaled;

MediaGeometry VideoParser::geometryFrom(const XmlNode& node)
{
    auto scaleType =
        node.get<MediaGeometry::ScaleType>(XlfResources::Media::Geometry::ScaleType, DefaultVideoScaleType);

    return MediaGeometry{scaleType, MediaGeometry::Align::Center, MediaGeometry::Valign::Middle};
}

std::unique_ptr<Xibo::Media> VideoParser::createMedia(const MediaOptions& baseOptions,
                                                      const XmlNode& node,
                                                      int width,
                                                      int height)
{
    auto muted = node.get<bool>(XlfResources::Player::Mute, DefaultVideoMuted);
    auto looped = node.get<bool>(XlfResources::Player::Loop, DefaultVideoLooped);
    MediaPlayerOptions options{baseOptions,
                               static_cast<MediaPlayerOptions::Mute>(muted),
                               static_cast<MediaPlayerOptions::Loop>(looped),
                               MaxVolume};

    VideoFactory factory;
    return factory.create(options, width, height);
}
