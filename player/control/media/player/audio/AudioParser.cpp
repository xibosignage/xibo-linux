#include "AudioParser.hpp"

#include "common/constants.hpp"
#include "control/media/Media.hpp"
#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/player/audio/AudioFactory.hpp"

const bool DefaultAudioLooped = false;

std::unique_ptr<Xibo::Media> AudioParser::createMedia(const MediaOptions& baseOptions,
                                                      const XmlNode& node,
                                                      int /*width*/,
                                                      int /*height*/)
{
    auto looped = node.get<bool>(XlfResources::Player::Loop, DefaultAudioLooped);
    auto volume = node.get<int>(XlfResources::Player::Volume, MaxVolume);
    MediaPlayerOptions options{
        baseOptions, MediaPlayerOptions::Mute::Disable, static_cast<MediaPlayerOptions::Loop>(looped), volume};

    AudioFactory factory;
    return factory.create(options);
}
