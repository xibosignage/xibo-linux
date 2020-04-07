#include "AudioNodeParser.hpp"

#include "control/media/Media.hpp"
#include "control/media/MediaResources.hpp"
#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/player/audio/AudioFactory.hpp"

#include "common/fs/FileSystem.hpp"
#include "common/fs/Resource.hpp"
#include "common/constants.hpp"

const bool DefaultAudioLooped = false;
const int DefaultDuration = 0;

MediaOptions::Type AudioNodeParser::typeFrom(const XmlNode& /*node*/)
{
    return {XlfResources::Media::AudioNodeType, XlfResources::Media::NativeRender};
}

int AudioNodeParser::idFrom(const XmlNode& node)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);
    return uriNode.get<int>(XlfResources::AudioNode::Id);
}

Uri AudioNodeParser::uriFrom(const XmlNode& node)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);
    auto uri = uriNode.get_value<std::string>();

    // TODO: remove duplicate
    Resource fullPath{uri};

    if (!FileSystem::isRegularFile(fullPath)) return Uri::fromString(uri);

    return Uri::fromFile(fullPath);
}

int AudioNodeParser::durationFrom(const XmlNode& /*node*/)
{
    return DefaultDuration;
}

std::unique_ptr<Xibo::Media> AudioNodeParser::createMedia(const MediaOptions& baseOptions,
                                                          const XmlNode& node,
                                                          int /*width*/,
                                                          int /*height*/)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);

    auto looped = uriNode.get<bool>(XlfResources::AudioNode::Loop, DefaultAudioLooped);
    auto volume = uriNode.get<int>(XlfResources::AudioNode::Volume, MaxVolume);
    MediaPlayerOptions options{
        baseOptions, MediaPlayerOptions::Mute::Disable, static_cast<MediaPlayerOptions::Loop>(looped), volume};

    AudioFactory factory;
    return factory.create(options);
}
