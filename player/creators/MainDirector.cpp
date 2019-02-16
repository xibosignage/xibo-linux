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
#include "utils/Utilities.hpp"

#include <boost/property_tree/ptree.hpp>

using namespace ResourcesXlf;

std::unique_ptr<IMainLayout> MainDirector::buildLayoutWithChildren(int layoutId)
{
    auto tree = Utils::parseXmlFromPath(getLayoutXlfPath(layoutId));

    return buildLayout(tree.get_child(ResourcesXlf::LayoutNode));
}

std::string MainDirector::getLayoutXlfPath(int layoutId)
{
    auto xlfFile = std::to_string(layoutId) + ".xlf";
    return Resources::directory() / xlfFile;
}

std::unique_ptr<IMainLayout> MainDirector::buildLayout(const xml_node& layoutNode)
{
    LayoutOptions opts{layoutNode};

    return MainLayoutBuilder().options(opts).background(buildBackground(layoutNode)).regions(collectRegions(layoutNode)).build();
}

std::unique_ptr<IBackground> MainDirector::buildBackground(const ResourcesXlf::BackgroundOptions& opts)
{
    return BackgroundBuilder().options(opts).build();
}

std::vector<RegionWithPos> MainDirector::collectRegions(const xml_node& layoutNode)
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

std::unique_ptr<IRegion> MainDirector::buildRegion(const xml_node& regionNode)
{
    RegionOptions opts{regionNode};
    auto allContent = collectContent(opts.width(), opts.height(), regionNode);

    return RegionBuilder().options(opts).content(std::move(allContent)).build();
}

std::vector<ContentWithPos> MainDirector::collectContent(int regionWidth, int regionHeight, const xml_node& regionNode)
{
    GetMediaPosition positionCalc(regionWidth, regionHeight);
    std::vector<ContentWithPos> media;
    for(auto [nodeName, mediaNode] : regionNode)
    {
        if(nodeName == ResourcesXlf::MediaNode)
        {
            auto builtMedia = buildMedia(regionWidth, regionHeight, mediaNode);
            auto [mediaXPos, mediaYPos] = positionCalc.getMediaPos(builtMedia.get());

            auto content = std::make_unique<RegionContent>(std::move(builtMedia), std::make_unique<TimerProvider>());
            attachAdditionalMedia(mediaNode, *content);

            media.emplace_back(ContentWithPos{std::move(content), mediaXPos, mediaYPos});
        }
    }
    return media;
}

std::unique_ptr<IMedia> MainDirector::buildMedia(int regionWidth, int regionHeight, const xml_node& mediaNode)
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
    return ImageBuilder().options(opts).width(regionWidth).height(regionHeight).build();
}

std::unique_ptr<Video> MainDirector::buildVideo(int regionWidth, int regionHeight, const VideoOptions& opts)
{
    return VideoBuilder().options(opts).width(regionWidth).height(regionHeight).build();
}

std::unique_ptr<Audio> MainDirector::buildAudio(const AudioOptions& opts)
{
    return AudioBuilder().options(opts).build();
}

std::unique_ptr<WebView> MainDirector::buildWebView(int regionWidth, int regionHeight, const WebViewOptions& opts)
{
    return WebViewBuilder().options(opts).width(regionWidth).height(regionHeight).build();
}

void MainDirector::attachAdditionalMedia(const xml_node& mediaNode, IRegionContent& content)
{
    for(auto [nodeName, additionalMediaNode] : mediaNode)
    {
        if(nodeName == ResourcesXlf::AudioType)
        {
            content.attachMedia(buildAudio(AudioOptions{additionalMediaNode, AudioNodeTag{}}));
        }
    }
}
