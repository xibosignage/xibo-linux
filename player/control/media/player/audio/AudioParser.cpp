#include "AudioParser.hpp"

#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/creators/MediaResources.hpp"

#include "control/common/Validators.hpp"

const bool DEFAULT_AUDIO_LOOPED = false;
const int DEFAULT_DURATION = 0;

ParsedMedia AudioParser::parse(const xml_node& node)
{
    bool isAdditionalNode = node.get_optional<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type)) ? false : true;

    if(isAdditionalNode)
    {
        return parseAdditionalNode(node);
    }
    else
    {
        return MediaParser::parse(node);
    }
}

ExtraOptions AudioParser::parseAdditonalOptions(const xml_node& node)
{
    auto looped = node.get<bool>(ResourcesXlf::option(ResourcesXlf::Player::Loop), DEFAULT_AUDIO_LOOPED);
    auto volume = node.get<int>(ResourcesXlf::option(ResourcesXlf::Player::Volume), MAX_VOLUME);

    return {
        {ResourcesXlf::Player::Loop, std::to_string(looped)},
        {ResourcesXlf::Player::Volume, std::to_string(volume)}
    };
}

ParsedMedia AudioParser::parseAdditionalNode(const xml_node& node)
{
    auto uriNode = node.get_child(ResourcesXlf::Media::Uri);
    auto id = uriNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Player::AudioId));
    auto uri = Validators::validateUri(uriNode.get_value<std::string>());
    MediaOptions::Type type{ResourcesXlf::Media::AudioType, ResourcesXlf::Media::NativeRender};

    ExtraOptions options;
    auto loop = uriNode.get<bool>(ResourcesXlf::attr(ResourcesXlf::Player::Loop), DEFAULT_AUDIO_LOOPED);
    options.emplace(ResourcesXlf::Player::Loop, std::to_string(loop));
    auto volume = uriNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Player::Volume), MAX_VOLUME);
    options.emplace(ResourcesXlf::Player::Volume, std::to_string(volume));

    return ParsedMedia{MediaOptions{type, id, uri, DEFAULT_DURATION, {}}, options, nullptr};
}
