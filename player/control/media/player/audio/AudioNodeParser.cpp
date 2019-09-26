#include "AudioNodeParser.hpp"

#include "control/media/MediaResources.hpp"
#include "control/media/player/MediaPlayerResources.hpp"

#include "control/common/Validators.hpp"

const bool DefaultAudioLooped = false;
const int DefaultDuration = 0;

MediaOptions::Type AudioNodeParser::typeFrom(const ptree_node& /*node*/)
{
    return {XlfResources::Media::AudioNodeType, XlfResources::Media::NativeRender};
}

int AudioNodeParser::idFrom(const ptree_node& node)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);
    return uriNode.get<int>(XlfResources::AudioNode::Id);
}

Uri AudioNodeParser::uriFrom(const ptree_node& node)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);
    return Validators::validateUri(uriNode.get_value<std::string>());
}

int AudioNodeParser::durationFrom(const ptree_node& /*node*/)
{
    return DefaultDuration;
}

ExtraOptions AudioNodeParser::extraOptionsImpl(const ptree_node& node)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);

    auto looped = uriNode.get<bool>(XlfResources::AudioNode::Loop, DefaultAudioLooped);
    auto volume = uriNode.get<int>(XlfResources::AudioNode::Volume, MaxVolume);

    return {{XlfResources::Player::Loop, std::to_string(looped)},
            {XlfResources::Player::Volume, std::to_string(volume)}};
}
