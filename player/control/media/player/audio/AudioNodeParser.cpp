#include "AudioNodeParser.hpp"

#include "control/media/Media.hpp"
#include "control/media/MediaResources.hpp"
#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/player/audio/AudioFactory.hpp"

#include "common/fs/FileSystem.hpp"
#include "common/types/ResourceFile.hpp"

const bool DefaultAudioLooped = false;
const int DefaultDuration = 0;

MediaOptions::Type AudioNodeParser::typeFrom(const PtreeNode& /*node*/)
{
    return {XlfResources::Media::AudioNodeType, XlfResources::Media::NativeRender};
}

int AudioNodeParser::idFrom(const PtreeNode& node)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);
    return uriNode.get<int>(XlfResources::AudioNode::Id);
}

Uri AudioNodeParser::uriFrom(const PtreeNode& node)
{
    auto uriNode = node.get_child(XlfResources::AudioNode::Uri);
    auto uri = uriNode.get_value<std::string>();

    // TODO: remove duplicate
    ResourceFile fullPath{uri};

    if (!FileSystem::isRegularFile(fullPath)) return Uri::fromString(uri);

    return Uri{fullPath};
}

int AudioNodeParser::durationFrom(const PtreeNode& /*node*/)
{
    return DefaultDuration;
}

std::unique_ptr<Xibo::Media> AudioNodeParser::createMedia(const MediaOptions& baseOptions,
                                                          const PtreeNode& node,
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
