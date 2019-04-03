#include "AudioParser.hpp"

#include "MediaPlayerResources.hpp"

#include "control/media/MediaResources.hpp"
#include "control/common/Validators.hpp"

const int DEFAULT_DURATION = 0;
const bool DEFAULT_AUDIO_MUTED = false;
const bool DEFAULT_AUDIO_LOOPED = false;

AudioParser::AudioParser(const xml_node& node) :
    MediaParser(node)
{
}

MediaPlayerOptions AudioParser::parse()
{
    auto options = baseOptions();
    auto muted = node().get<bool>(ResourcesXlf::option(ResourcesXlf::Player::Mute), DEFAULT_AUDIO_MUTED);
    auto looped = node().get<bool>(ResourcesXlf::option(ResourcesXlf::Player::Loop), DEFAULT_AUDIO_LOOPED);
    auto volume = node().get<int>(ResourcesXlf::option(ResourcesXlf::Player::Volume), MAX_VOLUME);

    return MediaPlayerOptions{options,
                              static_cast<MediaPlayerOptions::Mute>(muted),
                              static_cast<MediaPlayerOptions::Loop>(looped),
                              volume};
}

MediaPlayerOptions AudioParser::parseNode()
{
    auto uriNode = node().get_child(ResourcesXlf::Media::Uri);

    auto id = uriNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Media::Id));
    auto uri = uriNode.get_value<std::string>();
    auto muted = uriNode.get<bool>(ResourcesXlf::attr(ResourcesXlf::Player::Mute), DEFAULT_AUDIO_MUTED);
    auto looped = uriNode.get<bool>(ResourcesXlf::attr(ResourcesXlf::Player::Loop), DEFAULT_AUDIO_LOOPED);
    auto volume = uriNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Player::Volume), MAX_VOLUME);

    return MediaPlayerOptions{MediaOptions{id, Validators::validateUri(uri), DEFAULT_DURATION},
                              static_cast<MediaPlayerOptions::Mute>(muted),
                              static_cast<MediaPlayerOptions::Loop>(looped),
                              volume};
}

