#include "MainDirector.hpp"

#include "creators/ImageBuilder.hpp"
#include "creators/VideoBuilder.hpp"
#include "creators/AudioBuilder.hpp"
#include "creators/WebViewBuilder.hpp"

#include "creators/BackgroundBuilder.hpp"
#include "creators/MainLayoutBuilder.hpp"
#include "creators/RegionBuilder.hpp"

#include "control/IRegion.hpp"
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
                              .regions(collectRegions(layoutNode)).build();
}

std::unique_ptr<IBackground> MainDirector::buildBackground(const ResourcesXlf::LayoutOptions& opts)
{
    return BackgroundBuilder().width(opts.width()).height(opts.height())
                              .path(opts.backgroundPath()).color(opts.backgroundColor()).build();
}

std::vector<RegionWithPos> MainDirector::collectRegions(const xlf_node& layoutNode)
{
    std::vector<RegionWithPos> regions;
    for(auto [nodeName, regionNode] : layoutNode)
    {
        if(nodeName == ResourcesXlf::RegionNode)
        {
            RegionOptions opts{regionNode};
            regions.emplace_back(RegionWithPos{buildRegion(regionNode), opts.left(), opts.top()});
        }
    }
    return regions;
}

std::unique_ptr<IRegion> MainDirector::buildRegion(const xlf_node& regionNode)
{
    RegionOptions opts{regionNode};

    auto visibleMedia = collectVisibleMedia(opts.width(), opts.height(), regionNode);
    auto invisibleMedia = collectInvisibleMedia(regionNode);

    return RegionBuilder().id(opts.id()).width(opts.width()).height(opts.height()).zorder(opts.zindex()).loop(opts.loop())
                                  .visibleMedia(std::move(visibleMedia)).invisibleMedia(std::move(invisibleMedia)).build();
}

std::vector<MediaWithPos> MainDirector::collectVisibleMedia(int regionWidth, int regionHeight, const xlf_node& regionNode)
{
    std::vector<MediaWithPos> media;
    for(auto [nodeName, mediaNode] : regionNode)
    {
        if(nodeName == ResourcesXlf::MediaNode && MediaOptions::getType(mediaNode) != ResourcesXlf::AudioType)
        {
            auto builtMedia = buildMedia(regionWidth, regionHeight, mediaNode);

            GetMediaPosition visitor(regionWidth, regionHeight);
            builtMedia->apply(visitor);

            media.emplace_back(MediaWithPos{std::move(builtMedia), visitor.getMediaX(), visitor.getMediaY()});
        }
    }
    return media;
}

std::vector<std::unique_ptr<IMedia>> MainDirector::collectInvisibleMedia(const xlf_node& regionNode)
{
    std::vector<std::unique_ptr<IMedia>> media;
    for(auto [nodeName, mediaNode] : regionNode)
    {
        if(nodeName == ResourcesXlf::MediaNode && MediaOptions::getType(mediaNode) == ResourcesXlf::AudioType)
        {
            media.emplace_back(buildMedia(mediaNode));
        }
    }
    return media;
}

std::unique_ptr<IMedia> MainDirector::buildMedia(int regionWidth, int regionHeight, const xlf_node& mediaNode)
{
    auto type = MediaOptions::getType(mediaNode);

    MediaOptions opts{mediaNode};
    std::unique_ptr<MediaBuilder> builder;

    if(type == ResourcesXlf::ImageType)
        builder = prepareImageBuilder(regionWidth, regionHeight, mediaNode);
    else if(type == ResourcesXlf::VideoType)
        builder = prepareVideoBuilder(regionWidth, regionHeight, mediaNode);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        builder = prepareWebViewBuilder(regionWidth, regionHeight, mediaNode);

    builder->id(opts.id()).path(opts.uri()).duration(opts.duration());
    attachAdditionalMedia(mediaNode, *builder);
    return builder->build();
}

std::unique_ptr<MediaBuilder> MainDirector::prepareImageBuilder(int regionWidth, int regionHeight, const ImageOptions& opts)
{
    auto builder = std::make_unique<ImageBuilder>();
    builder->width(regionWidth).height(regionHeight).scaleType(opts.scaleType()).align(opts.align()).valign(opts.valign());
    return builder;
}

std::unique_ptr<MediaBuilder> MainDirector::prepareVideoBuilder(int regionWidth, int regionHeight, const VideoOptions& opts)
{
    auto builder = std::make_unique<VideoBuilder>();
    builder->width(regionWidth).height(regionHeight).muted(opts.muted()).looped(opts.looped());
    return builder;
}

std::unique_ptr<MediaBuilder> MainDirector::prepareWebViewBuilder(int regionWidth, int regionHeight, const WebViewOptions& opts)
{
    auto builder = std::make_unique<WebViewBuilder>();
    builder->width(regionWidth).height(regionHeight).transparent(opts.transparent());
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
