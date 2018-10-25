#include "MainBuilder.hpp"

#include "creators/ImageBuilder.hpp"
#include "creators/VideoBuilder.hpp"
#include "creators/AudioBuilder.hpp"
#include "creators/WebViewBuilder.hpp"

#include "creators/BackgroundBuilder.hpp"
#include "creators/MainLayoutBuilder.hpp"
#include "creators/MediaContainerBuilder.hpp"

#include "media/GetMediaPosition.hpp"
#include "utils/Resources.hpp"

using namespace ResourcesXlf;

std::unique_ptr<IMainLayout> MainBuilder::buildLayoutWithChildren(const xlf_node& tree)
{
    return buildLayout(tree.get_child(ResourcesXlf::LayoutNode));
}

std::unique_ptr<IMainLayout> MainBuilder::buildLayout(const xlf_node& layoutNode)
{
    LayoutOptions opts{layoutNode};

    return MainLayoutBuilder().width(opts.width).height(opts.height).background(buildBackground(opts))
                              .mediaContainers(collectContainers(layoutNode)).build();
}

std::unique_ptr<IBackground> MainBuilder::buildBackground(const ResourcesXlf::LayoutOptions& opts)
{
    return BackgroundBuilder().width(opts.width).height(opts.height)
                              .path(opts.backgroundPath).color(opts.backgroundColor).build();
}

std::vector<MediaContainerWithPos> MainBuilder::collectContainers(const xlf_node& layoutNode)
{
    std::vector<MediaContainerWithPos> containers;
    for(auto [nodeName, containerNode] : layoutNode)
    {
        if(nodeName == ResourcesXlf::RegionNode)
        {
            RegionOptions opts{containerNode};
            containers.emplace_back(MediaContainerWithPos{buildContainer(containerNode), opts.top, opts.left});
        }
    }
    return containers;
}

std::unique_ptr<IMediaContainer> MainBuilder::buildContainer(const xlf_node& containerNode)
{
    RegionOptions opts{containerNode};

    auto visibleMedia = collectVisibleMedia(opts.width, opts.height, containerNode);
    auto invisibleMedia = collectInvisibleMedia(containerNode);

    return MediaContainerBuilder().width(opts.width).height(opts.height).zorder(opts.zindex).loop(opts.loop)
                                  .visibleMedia(std::move(visibleMedia)).invisibleMedia(std::move(invisibleMedia)).build();
}

std::vector<MediaWithPos> MainBuilder::collectVisibleMedia(int containerWidth, int containerHeight, const xlf_node& containerNode)
{
    std::vector<MediaWithPos> media;
    for(auto [nodeName, mediaNode] : containerNode)
    {
        if(nodeName == ResourcesXlf::MediaNode && MediaOptions::getType(mediaNode) != ResourcesXlf::AudioType) // FIXME condition
        {
            auto builtMedia = buildMedia(containerWidth, containerHeight, mediaNode);

            GetMediaPosition visitor(containerWidth, containerHeight); // WARNING change from visitor (?)
            builtMedia->apply(visitor);

            media.emplace_back(MediaWithPos{std::move(builtMedia), visitor.getMediaX(), visitor.getMediaY()});
        }
    }
    return media;
}

std::vector<std::unique_ptr<IMedia>> MainBuilder::collectInvisibleMedia(const xlf_node& containerNode)
{
    std::vector<std::unique_ptr<IMedia>> media;
    for(auto [nodeName, mediaNode] : containerNode)
    {
        if(nodeName == ResourcesXlf::MediaNode && MediaOptions::getType(mediaNode) == ResourcesXlf::AudioType) // FIXME condition
        {
            media.emplace_back(buildMedia(mediaNode));
        }
    }
    return media;
}

std::unique_ptr<IMedia> MainBuilder::buildMedia(int containerWidth, int containerHeight, const xlf_node& mediaNode)
{
    auto type = MediaOptions::getType(mediaNode);

    if(type == ResourcesXlf::ImageType)
        return buildImage(containerWidth, containerHeight, mediaNode);
    else if(type == ResourcesXlf::VideoType)
        return buildVideo(containerWidth, containerHeight, mediaNode);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        return buildWebView(containerWidth, containerHeight, mediaNode);
}

std::unique_ptr<IMedia> MainBuilder::buildImage(int containerWidth, int containerHeight, const ImageOptions& opts)
{
    return ImageBuilder().width(containerWidth).height(containerHeight).scaleType(opts.scaleType).align(opts.align)
                         .valign(opts.valign).path(opts.uri).duration(opts.duration).build();
}

std::unique_ptr<IMedia> MainBuilder::buildVideo(int containerWidth, int containerHeight, const VideoOptions& opts)
{
    return VideoBuilder().width(containerWidth).height(containerHeight).muted(opts.muted).looped(opts.looped)
                         .path(opts.uri).duration(opts.duration).build();
}

std::unique_ptr<IMedia> MainBuilder::buildWebView(int containerWidth, int containerHeight, const WebViewOptions& opts)
{
    return WebViewBuilder().width(containerWidth).height(containerHeight).transparent(opts.transparent)
                           .id(opts.id).path(opts.uri).duration(opts.duration).build();
}

std::unique_ptr<IMedia> MainBuilder::buildMedia(const xlf_node& mediaNode)
{
    auto type = MediaOptions::getType(mediaNode);

    if(type == ResourcesXlf::AudioType)
        return buildAudio(mediaNode);

    return nullptr;
}

std::unique_ptr<IMedia> MainBuilder::buildAudio(const AudioOptions& opts)
{
    return AudioBuilder().muted(opts.muted).looped(opts.looped).volume(opts.volume).path(opts.uri).duration(opts.duration).build();
}

