#include "MainDirector.hpp"

#include "creators/ImageBuilder.hpp"
#include "creators/VideoBuilder.hpp"
#include "creators/AudioBuilder.hpp"
#include "creators/WebViewBuilder.hpp"

#include "creators/BackgroundBuilder.hpp"
#include "creators/MainLayoutBuilder.hpp"
#include "creators/MediaContainerBuilder.hpp"

#include "control/IMediaContainer.hpp"
#include "control/IBackground.hpp"
#include "control/IMainLayout.hpp"
#include "media/IMedia.hpp"
#include "media/GetMediaPosition.hpp"
#include "parsers/XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

using namespace ResourcesXlf;

std::unique_ptr<IMainLayout> MainDirector::buildLayoutWithChildren(const xlf_node& tree)
{
    return buildLayout(tree.get_child(ResourcesXlf::LayoutNode));
}

std::unique_ptr<IMainLayout> MainDirector::buildLayout(const xlf_node& layoutNode)
{
    LayoutOptions opts{layoutNode};

    return MainLayoutBuilder().width(opts.width()).height(opts.height()).background(buildBackground(opts))
                              .mediaContainers(collectContainers(layoutNode)).build();
}

std::unique_ptr<IBackground> MainDirector::buildBackground(const ResourcesXlf::LayoutOptions& opts)
{
    return BackgroundBuilder().width(opts.width()).height(opts.height())
                              .path(opts.backgroundPath()).color(opts.backgroundColor()).build();
}

std::vector<MediaContainerWithPos> MainDirector::collectContainers(const xlf_node& layoutNode)
{
    std::vector<MediaContainerWithPos> containers;
    for(auto [nodeName, containerNode] : layoutNode)
    {
        if(nodeName == ResourcesXlf::RegionNode)
        {
            RegionOptions opts{containerNode};
            containers.emplace_back(MediaContainerWithPos{buildContainer(containerNode), opts.left(), opts.top()});
        }
    }
    return containers;
}

std::unique_ptr<IMediaContainer> MainDirector::buildContainer(const xlf_node& containerNode)
{
    RegionOptions opts{containerNode};

    auto visibleMedia = collectVisibleMedia(opts.width(), opts.height(), containerNode);
    auto invisibleMedia = collectInvisibleMedia(containerNode);

    return MediaContainerBuilder().id(opts.id()).width(opts.width()).height(opts.height()).zorder(opts.zindex()).loop(opts.loop())
                                  .visibleMedia(std::move(visibleMedia)).invisibleMedia(std::move(invisibleMedia)).build();
}

std::vector<MediaWithPos> MainDirector::collectVisibleMedia(int containerWidth, int containerHeight, const xlf_node& containerNode)
{
    std::vector<MediaWithPos> media;
    for(auto [nodeName, mediaNode] : containerNode)
    {
        if(nodeName == ResourcesXlf::MediaNode && MediaOptions::getType(mediaNode) != ResourcesXlf::AudioType)
        {
            auto builtMedia = buildMedia(containerWidth, containerHeight, mediaNode);

            GetMediaPosition visitor(containerWidth, containerHeight);
            builtMedia->apply(visitor);

            media.emplace_back(MediaWithPos{std::move(builtMedia), visitor.getMediaX(), visitor.getMediaY()});
        }
    }
    return media;
}

std::vector<std::unique_ptr<IMedia>> MainDirector::collectInvisibleMedia(const xlf_node& containerNode)
{
    std::vector<std::unique_ptr<IMedia>> media;
    for(auto [nodeName, mediaNode] : containerNode)
    {
        if(nodeName == ResourcesXlf::MediaNode && MediaOptions::getType(mediaNode) == ResourcesXlf::AudioType)
        {
            media.emplace_back(buildMedia(mediaNode));
        }
    }
    return media;
}

std::unique_ptr<IMedia> MainDirector::buildMedia(int containerWidth, int containerHeight, const xlf_node& mediaNode)
{
    auto type = MediaOptions::getType(mediaNode);

    MediaOptions opts{mediaNode};
    std::unique_ptr<MediaBuilder> builder;

    if(type == ResourcesXlf::ImageType)
        builder = prepareImageBuilder(containerWidth, containerHeight, mediaNode);
    else if(type == ResourcesXlf::VideoType)
        builder = prepareVideoBuilder(containerWidth, containerHeight, mediaNode);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        builder = prepareWebViewBuilder(containerWidth, containerHeight, mediaNode);

    builder->id(opts.id()).path(opts.uri()).duration(opts.duration());
    attachAdditionalMedia(mediaNode, *builder);
    return builder->build();
}

std::unique_ptr<MediaBuilder> MainDirector::prepareImageBuilder(int containerWidth, int containerHeight, const ImageOptions& opts)
{
    auto builder = std::make_unique<ImageBuilder>();
    builder->width(containerWidth).height(containerHeight).scaleType(opts.scaleType()).align(opts.align()).valign(opts.valign());
    return builder;
}

std::unique_ptr<MediaBuilder> MainDirector::prepareVideoBuilder(int containerWidth, int containerHeight, const VideoOptions& opts)
{
    auto builder = std::make_unique<VideoBuilder>();
    builder->width(containerWidth).height(containerHeight).muted(opts.muted()).looped(opts.looped());
    return builder;
}

std::unique_ptr<MediaBuilder> MainDirector::prepareWebViewBuilder(int containerWidth, int containerHeight, const WebViewOptions& opts)
{
    auto builder = std::make_unique<WebViewBuilder>();
    builder->width(containerWidth).height(containerHeight).transparent(opts.transparent());
    return builder;
}

void MainDirector::attachAdditionalMedia(const xlf_node& mediaNode, MediaBuilder& builder)
{
    for(auto [nodeName, additionalMediaNode] : mediaNode)
    {
        if(nodeName == ResourcesXlf::AudioType)
        {
            builder.audio(buildAudio(AudioOptions{additionalMediaNode, AudioNodeTag{}}));
        }
    }
}

std::unique_ptr<IMedia> MainDirector::buildMedia(const xlf_node& mediaNode)
{
    auto type = MediaOptions::getType(mediaNode);

    if(type == ResourcesXlf::AudioType)
        return buildAudio(mediaNode);

    return nullptr;
}

std::unique_ptr<IMedia> MainDirector::buildAudio(const AudioOptions& opts)
{
    return AudioBuilder().muted(opts.muted()).looped(opts.looped()).volume(opts.volume()).path(opts.uri()).duration(opts.duration()).build();
}
