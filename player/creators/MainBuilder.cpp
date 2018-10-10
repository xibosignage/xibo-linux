#include "MainBuilder.hpp"

#include "creators/ImageFactory.hpp"
#include "creators/VideoFactory.hpp"
#include "creators/AudioFactory.hpp"
#include "creators/WebViewFactory.hpp"

#include "creators/BackgroundBuilder.hpp"
#include "adaptors/GtkImageAdaptor.hpp"
#include "creators/MainLayoutBuilder.hpp"
#include "adaptors/GtkOverlayAdaptor.hpp"
#include "creators/MediaContainerBuilder.hpp"
#include "adaptors/GtkFixedLayoutAdaptor.hpp"

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

    MainLayoutBuilder builder;
    return builder.adaptor(std::make_unique<GtkOverlayAdaptor>()).width(width).height(height)
                  .background(buildBackground(layoutNode)).mediaContainers(collectContainers(layoutNode)).build();
}

std::unique_ptr<IBackground> MainBuilder::buildBackground(const xlf_node& layoutNode)
{
    int width = LayoutXlf::width(layoutNode);
    int height = LayoutXlf::height(layoutNode);
    auto path = LayoutXlf::backgroundPath(layoutNode);
    auto color = LayoutXlf::backgroundColor(layoutNode);

    BackgroundBuilder builder{std::make_unique<FileSystemAdaptor>()};
    return builder.adaptor(std::make_unique<GtkImageAdaptor>()).width(width).height(height).path(path).color(color).build();
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

    MediaContainerBuilder builder;
    return builder.adaptor(std::make_unique<GtkFixedLayoutAdaptor>())
                  .width(width).height(height).zorder(zorder).loop(loop)
                  .media(collectMedia(containerNode)).build();
}

std::vector<MediaStruct> MainBuilder::collectMedia(const xlf_node& containerNode)
{
    std::vector<MediaStruct> media;
    for(auto [nodeName, mediaNode] : containerNode)
    {
        if(nodeName == MediaXlf::NodeName)
        {
            auto type = MediaXlf::type(mediaNode);
            auto builtMedia = buildMedia(containerNode, mediaNode);
            int containerWidth = RegionXlf::width(containerNode);
            int containerHeight = RegionXlf::height(containerNode);

            GetMediaPosition visitor(containerWidth, containerHeight);
            builtMedia->apply(visitor);

            media.emplace_back(MediaStruct{std::move(builtMedia), type, visitor.getMediaX(), visitor.getMediaY()});
        }
    }
    return media;
}

std::unique_ptr<IMedia> MainBuilder::buildMedia(const xlf_node& containerNode, const xlf_node& mediaNode)
{
    auto type = MediaXlf::type(mediaNode);

    std::unique_ptr<MediaFactory> factory;

    if(type == MediaXlf::ImageType)
        factory = std::make_unique<ImageFactory>(containerNode, mediaNode);
    else if(type == MediaXlf::VideoType)
        factory = std::make_unique<VideoFactory>(containerNode, mediaNode);
    else if(type == MediaXlf::AudioType)
        factory = std::make_unique<AudioFactory>(containerNode, mediaNode);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        factory = std::make_unique<WebViewFactory>(containerNode, mediaNode);

    return factory->create();
}
