#include "AudioParser.hpp"

#include "control/media/player/MediaPlayerResources.hpp"

#include "control/media/MediaResources.hpp"
#include "control/common/Validators.hpp"

const int DEFAULT_DURATION = 0;
const bool DEFAULT_AUDIO_MUTED = false;
const bool DEFAULT_AUDIO_LOOPED = false;

AudioParser::AudioParser(const xml_node& node) :
    MediaParser(node)
{
    m_isAdditionalNode = node.get_optional<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type)) ? false : true;
}

MediaPlayerOptions AudioParser::parse()
{
    if(m_isAdditionalNode)
    {
        return parseAdditionalNode();
    }
    else
    {
        return parseMainNode();
    }
}

MediaPlayerOptions AudioParser::parseMainNode()
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

MediaPlayerOptions AudioParser::parseAdditionalNode()
{
    auto uriNode = node().get_child(ResourcesXlf::Media::Uri);

    auto id = uriNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Player::AudioId));
    auto uri = uriNode.get_value<std::string>();
    auto muted = uriNode.get<bool>(ResourcesXlf::attr(ResourcesXlf::Player::Mute), DEFAULT_AUDIO_MUTED);
    auto looped = uriNode.get<bool>(ResourcesXlf::attr(ResourcesXlf::Player::Loop), DEFAULT_AUDIO_LOOPED);
    auto volume = uriNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Player::Volume), MAX_VOLUME);

    return MediaPlayerOptions{MediaOptions{id, Validators::validateUri(uri), DEFAULT_DURATION, {}},
                              static_cast<MediaPlayerOptions::Mute>(muted),
                              static_cast<MediaPlayerOptions::Loop>(looped),
                              volume};
}

