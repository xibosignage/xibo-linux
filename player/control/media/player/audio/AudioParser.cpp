#include "AudioParser.hpp"

#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/MediaResources.hpp"

#include "control/common/Validators.hpp"

const bool DefaultAudioLooped = false;
const int DefaultDuration = 0;

std::unique_ptr<IMedia> AudioParser::mediaFrom(const xml_node& node)
{
    bool isAdditionalNode = node.get_optional<std::string>(ResourcesXlf::Media::Type) ? false : true;

    if(isAdditionalNode)
    {
        return parseAdditionalNode(node);
    }

    return MediaParser::mediaFrom(node);
}

ExtraOptions AudioParser::parseExtraOptionsImpl(const xml_node& node)
{
    auto looped = node.get<bool>(ResourcesXlf::Player::Loop, DefaultAudioLooped);
    auto volume = node.get<int>(ResourcesXlf::Player::Volume, MaxVolume);

    return {
        {ResourcesXlf::Player::Loop, std::to_string(looped)},
        {ResourcesXlf::Player::Volume, std::to_string(volume)}
    };
}

std::unique_ptr<IMedia> AudioParser::parseAdditionalNode(const xml_node& node)
{
    auto uriNode = node.get_child(ResourcesXlf::AudioNode::Uri);
    auto id = uriNode.get<int>(ResourcesXlf::AudioNode::Id);
    auto uri = Validators::validateUri(uriNode.get_value<std::string>());
    MediaOptions::Type type{ResourcesXlf::Media::AudioType, ResourcesXlf::Media::NativeRender};

    ExtraOptions options;
    auto loop = uriNode.get<bool>(ResourcesXlf::AudioNode::Loop, DefaultAudioLooped);
    options.emplace(ResourcesXlf::Player::Loop, std::to_string(loop));
    auto volume = uriNode.get<int>(ResourcesXlf::AudioNode::Volume, MaxVolume);
    options.emplace(ResourcesXlf::Player::Volume, std::to_string(volume));

//    return ParsedMedia{MediaOptions{type, id, uri, DefaultDuration, {}, {}, {}}, options, nullptr};
}
