#include "AudioParser.hpp"

#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/MediaResources.hpp"

#include "control/common/Validators.hpp"

const bool DefaultAudioLooped = false;
const int DefaultDuration = 0;

std::unique_ptr<IMedia> AudioParser::mediaFrom(const ptree_node& node)
{
    bool isAdditionalNode = node.get_optional<std::string>(XlfResources::Media::Type) ? false : true;

    if(isAdditionalNode)
    {
        return parseAdditionalNode(node);
    }

    return MediaParser::mediaFrom(node);
}

ExtraOptions AudioParser::parseExtraOptionsImpl(const ptree_node& node)
{
    auto looped = node.get<bool>(XlfResources::Player::Loop, DefaultAudioLooped);
    auto volume = node.get<int>(XlfResources::Player::Volume, MaxVolume);

    return {
        {XlfResources::Player::Loop, std::to_string(looped)},
        {XlfResources::Player::Volume, std::to_string(volume)}
    };
}

std::unique_ptr<IMedia> AudioParser::parseAdditionalNode(const ptree_node& node)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);
    auto id = uriNode.get<int>(XlfResources::AudioNode::Id);
    auto uri = Validators::validateUri(uriNode.get_value<std::string>());
    MediaOptions::Type type{XlfResources::Media::AudioType, XlfResources::Media::NativeRender};

    ExtraOptions options;
    auto loop = uriNode.get<bool>(XlfResources::AudioNode::Loop, DefaultAudioLooped);
    options.emplace(XlfResources::Player::Loop, std::to_string(loop));
    auto volume = uriNode.get<int>(XlfResources::AudioNode::Volume, MaxVolume);
    options.emplace(XlfResources::Player::Volume, std::to_string(volume));

//    return ParsedMedia{MediaOptions{type, id, uri, DefaultDuration, {}, {}, {}}, options, nullptr};
}
