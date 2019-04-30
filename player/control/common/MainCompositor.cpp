#include "MainCompositor.hpp"

#include "XlfResources.hpp"

#include "common/Utils.hpp"
#include "utils/Resources.hpp"

#include "control/layout/MainLayoutParser.hpp"
#include "control/layout/MainLayoutView.hpp"

#include "control/region/RegionParser.hpp"
#include "control/region/RegionView.hpp"

#include "control/media/GetMediaPosition.hpp"
#include "control/media/MediaResources.hpp"
#include "control/media/MediaFactory.hpp"

#include "control/common/Image.hpp"
#include "control/media/image/ImageFactory.hpp"
#include "control/media/image/ImageParser.hpp"

#include "control/media/webview/WebViewParser.hpp"
#include "control/media/webview/WebViewFactory.hpp"

#include "control/media/player/audio/AudioParser.hpp"
#include "control/media/player/audio/AudioFactory.hpp"
#include "control/media/player/video/VideoParser.hpp"
#include "control/media/player/video/VideoFactory.hpp"

std::unique_ptr<MainLayout> MainCompositor::parseLayout(int layoutId)
{
    auto tree = Utils::parseXmlFromPath(getXlfFilePath(layoutId));
    auto layoutNode = tree.get_child(ResourcesXlf::LayoutNode);

    return parseLayout(layoutNode);
}

FilePath MainCompositor::getXlfFilePath(int layoutId)
{
    auto xlfFile = std::to_string(layoutId) + ".xlf";
    return Resources::resDirectory() / xlfFile;
}

std::unique_ptr<MainLayout> MainCompositor::parseLayout(const xml_node& layoutNode)
{
    auto options = parse<MainLayoutParser>(layoutNode);
    auto layout = std::make_unique<MainLayout>(createLayoutView(options));

    addRegions(*layout, layoutNode);

    return layout;
}

std::shared_ptr<MainLayoutView> MainCompositor::createLayoutView(const MainLayoutOptions& options)
{
    auto layoutView = std::make_shared<MainLayoutView>(options.width, options.height);

    layoutView->addBackground(createBackground(options));

    return layoutView;
}

std::shared_ptr<Widget> MainCompositor::createBackground(const MainLayoutOptions& options)
{
    auto background = std::make_shared<Image>(options.width, options.height);

    if(options.backgroundUri.isValid())
        background->loadFromFile(options.backgroundUri, false);
    else
        background->setColor(options.backgroundColor);

    return background;
}

void MainCompositor::addRegions(MainLayout& layout, const xml_node& node)
{
    for(auto [nodeName, regionNode] : node)
    {
        if(nodeName == ResourcesXlf::RegionNode)
        {
            auto options = parse<RegionParser>(regionNode);
            auto regionView = std::make_shared<RegionView>(options.width, options.height);
            auto region = std::make_unique<Region>(options, regionView);

            addAllMedia(*region, regionNode);

            layout.addRegion(std::move(region), options.left, options.top, options.zindex);
        }
    }
}

void MainCompositor::addAllMedia(Region& region,const xml_node& node)
{
    for(auto [nodeName, mediaNode] : node)
    {
        if(nodeName == ResourcesXlf::MediaNode)
        {
            addMedia(region, mediaNode);
        }
    }
}

void MainCompositor::addMedia(Region& region, const xml_node& node)
{
    auto type = node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type));
    auto view = region.view();

    auto adder = createMediaFactory(type, view->width(), view->height(), node);
    auto media = adder->create();

    attachAdditionalMedia(*media, node);

    region.addMedia(std::move(media));
}

std::unique_ptr<MediaFactory> MainCompositor::createMediaFactory(const std::string& type, int width, int height, const xml_node& node)
{
    if(type == ResourcesXlf::ImageType)
        return std::make_unique<ImageFactory>(width, height, parse<ImageParser>(node));
    else if(type == ResourcesXlf::VideoType)
        return std::make_unique<VideoFactory>(width, height, parse<VideoParser>(node));
    else if(type == ResourcesXlf::AudioType)
        return std::make_unique<AudioFactory>(parse<AudioParser>(node));
    else
        return std::make_unique<WebViewFactory>(width, height, parse<WebViewParser>(node));
}

void MainCompositor::attachAdditionalMedia(Media& media, const xml_node& node)
{
    for(auto [nodeName, attachedNode] : node)
    {
        if(nodeName == ResourcesXlf::AudioType)
        {
            auto factory = createMediaFactory(nodeName, 0, 0, attachedNode);
            media.attachMedia(factory->create());
        }
    }
}
