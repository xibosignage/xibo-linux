#include "MainDirector.hpp"

#include "creators/BackgroundBuilder.hpp"
#include "creators/MainLayoutBuilder.hpp"
#include "creators/RegionBuilder.hpp"

#include "control/IRegion.hpp"
#include "control/IBackground.hpp"
#include "control/IMainLayout.hpp"
#include "control/RegionContent.hpp"

#include "creators/ImageBuilder.hpp"
#include "creators/VideoBuilder.hpp"
#include "creators/AudioBuilder.hpp"
#include "creators/WebViewBuilder.hpp"

#include "media/GetMediaPosition.hpp"
#include "parsers/XlfResources.hpp"
#include "utils/TimerProvider.hpp"

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
    auto allContent = collectContent(opts.width(), opts.height(), regionNode);

    return RegionBuilder().id(opts.id()).width(opts.width()).height(opts.height()).zorder(opts.zindex()).loop(opts.loop()).content(std::move(allContent)).build();
}

std::vector<ContentWithPos> MainDirector::collectContent(int regionWidth, int regionHeight, const xlf_node& regionNode)
{
    std::vector<ContentWithPos> media;
    for(auto [nodeName, mediaNode] : regionNode)
    {
        if(nodeName == ResourcesXlf::MediaNode)
        {
            auto builtMedia = buildMedia(regionWidth, regionHeight, mediaNode);

            GetMediaPosition visitor(regionWidth, regionHeight);
            builtMedia->apply(visitor);

            auto content = std::make_unique<RegionContent>(std::move(builtMedia), std::make_unique<TimerProvider>());
            attachAdditionalMedia(mediaNode, *content);

            media.emplace_back(ContentWithPos{std::move(content), visitor.getMediaX(), visitor.getMediaY()});
        }
    }
    return media;
}

std::unique_ptr<IMedia> MainDirector::buildMedia(int regionWidth, int regionHeight, const xlf_node& mediaNode)
{
    auto type = MediaOptions::getType(mediaNode);

    if(type == ResourcesXlf::ImageType)
        return buildImage(regionWidth, regionHeight, mediaNode);
    else if(type == ResourcesXlf::VideoType)
        return buildVideo(regionWidth, regionHeight, mediaNode);
    else if(type == ResourcesXlf::AudioType)
        return buildAudio(mediaNode);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        return buildWebView(regionWidth, regionHeight, mediaNode);
}

std::unique_ptr<Image> MainDirector::buildImage(int regionWidth, int regionHeight, const ImageOptions& opts)
{
    return ImageBuilder().width(regionWidth).height(regionHeight).scaleType(opts.scaleType()).align(opts.align()).valign(opts.valign()).id(opts.id()).path(opts.uri()).duration(opts.duration()).build();
}

std::unique_ptr<Video> MainDirector::buildVideo(int regionWidth, int regionHeight, const VideoOptions& opts)
{
    return VideoBuilder().width(regionWidth).height(regionHeight).muted(opts.muted()).looped(opts.looped()).id(opts.id()).path(opts.uri()).duration(opts.duration()).build();
}

std::unique_ptr<Audio> MainDirector::buildAudio(const AudioOptions& opts)
{
    return AudioBuilder().muted(opts.muted()).looped(opts.looped()).volume(opts.volume()).id(opts.id()).path(opts.uri()).duration(opts.duration()).build();
}

std::unique_ptr<WebView> MainDirector::buildWebView(int regionWidth, int regionHeight, const WebViewOptions& opts)
{
    return WebViewBuilder().width(regionWidth).height(regionHeight).transparent(opts.transparent()).id(opts.id()).path(opts.uri()).duration(opts.duration()).build();
}

void MainDirector::attachAdditionalMedia(const xlf_node& mediaNode, IRegionContent& content)
{
    for(auto [nodeName, additionalMediaNode] : mediaNode)
    {
        if(nodeName == ResourcesXlf::AudioType)
        {
            content.attachMedia(buildAudio(AudioOptions{additionalMediaNode, AudioNodeTag{}}));
        }
    }
}
