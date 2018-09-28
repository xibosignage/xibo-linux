#include "MainBuilder.hpp"

#include "creators/MainLayoutBuilder.hpp"
#include "creators/BackgroundBuilder.hpp"
#include "creators/MediaContainerBuilder.hpp"

#include "creators/ImageFactory.hpp"
#include "creators/VideoFactory.hpp"
#include "creators/AudioFactory.hpp"
#include "creators/WebViewFactory.hpp"

#include "creators/ConcreteFactories.hpp"

#include "media/GetMediaPosition.hpp"
#include "parsers/Resources.hpp"

namespace LayoutXlf = ResourcesXlf::Layout;
namespace RegionXlf = ResourcesXlf::Region;
namespace MediaXlf = ResourcesXlf::Media;

std::unique_ptr<IMainLayout> MainBuilder::buildLayoutWithChildren(const xlf_node& tree)
{
    return buildLayout(tree.get_child(LayoutXlf::NodeName));
}

std::unique_ptr<IMainLayout> MainBuilder::buildLayout(const xlf_node& layoutNode)
{
    int width = LayoutXlf::width(layoutNode);
    int height = LayoutXlf::height(layoutNode);

    return MainLayoutBuilder().setWidth(width)
                              .setHeight(height)
                              .setBackground(buildBackground(layoutNode))
                              .setMediaContainers(collectContainers(layoutNode))
                              .build<MainLayoutFactory>();
}

std::unique_ptr<IBackground> MainBuilder::buildBackground(const xlf_node& layoutNode)
{
    int width = LayoutXlf::width(layoutNode);
    int height = LayoutXlf::height(layoutNode);
    auto path = LayoutXlf::backgroundPath(layoutNode);
    auto color = LayoutXlf::backgroundColor(layoutNode);

    return BackgroundBuilder().setWidth(width)
                              .setHeight(height)
                              .setPath(path)
                              .setColor(color)
                              .build<BackgroundFactory>();
}


std::vector<MediaContainerStruct> MainBuilder::collectContainers(const xlf_node& layoutNode)
{
    std::vector<MediaContainerStruct> containers;
    for(auto [nodeName, containerNode] : layoutNode)
    {
        if(nodeName == RegionXlf::NodeName)
        {
            int x = RegionXlf::left(containerNode);
            int y = RegionXlf::top(containerNode);
            containers.emplace_back(MediaContainerStruct{buildContainer(containerNode), x, y});
        }
    }
    return containers;
}

std::unique_ptr<IMediaContainer> MainBuilder::buildContainer(const xlf_node& containerNode)
{
    int width = RegionXlf::width(containerNode);
    int height = RegionXlf::height(containerNode);
    auto zorder = RegionXlf::zindex(containerNode);
    auto loop = RegionXlf::loop(containerNode);

    return MediaContainerBuilder().setWidth(width)
                                  .setHeight(height)
                                  .setZorder(zorder)
                                  .setLoop(loop)
                                  .setMedia(collectMedia(containerNode))
                                  .build<MediaContainerFactory>();
}

std::vector<MediaStruct> MainBuilder::collectMedia(const xlf_node& containerNode)
{
    std::vector<MediaStruct> media;
    for(auto [nodeName, mediaNode] : containerNode)
    {
        if(nodeName == MediaXlf::NodeName)
        {
            auto type = MediaXlf::type(mediaNode);
            auto builtMedia = buildMedia(mediaNode);
            int containerWidth = RegionXlf::width(containerNode);
            int containerHeight = RegionXlf::height(containerNode);

            GetMediaPosition visitor(containerWidth, containerHeight);
            builtMedia->apply(visitor);

            media.emplace_back(MediaStruct{std::move(builtMedia), type, visitor.getMediaX(), visitor.getMediaY()});
        }
    }
    return media;
}

std::unique_ptr<IMedia> MainBuilder::buildMedia(const xlf_node& mediaNode)
{
    auto type = MediaXlf::type(mediaNode);

    std::unique_ptr<MediaFactory> factory;

    if(type == MediaXlf::ImageType)
        factory = std::make_unique<ImageFactory>(mediaNode);
    else if(type == MediaXlf::VideoType)
        factory = std::make_unique<VideoFactory>(mediaNode);
    else if(type == MediaXlf::AudioType)
        factory = std::make_unique<AudioFactory>(mediaNode);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        factory = std::make_unique<WebViewFactory>(mediaNode);

    return factory->create();
}
