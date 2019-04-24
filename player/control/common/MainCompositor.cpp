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

std::pair<std::unique_ptr<MainLayout>, std::shared_ptr<MainLayoutView>> MainCompositor::parseLayout(int layoutId)
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

std::pair<std::unique_ptr<MainLayout>, std::shared_ptr<MainLayoutView>> MainCompositor::parseLayout(const xml_node& layoutNode)
{
    auto options = parse<MainLayoutParser>(layoutNode);
    auto layout = std::make_unique<MainLayout>();
    auto layoutView = std::make_shared<MainLayoutView>(options.width, options.height);

    layoutView->addBackground(createBackground(options));
    addRegions(*layout, *layoutView, layoutNode);

    return std::pair{std::move(layout), layoutView};
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

void MainCompositor::addRegions(MainLayout& layout, MainLayoutView& view, const xml_node& node)
{
    for(auto [nodeName, regionNode] : node)
    {
        if(nodeName == ResourcesXlf::RegionNode)
        {
            auto options = parse<RegionParser>(regionNode);
            auto region = std::make_unique<Region>(options);
            auto regionView = std::make_shared<RegionView>(options.width, options.height);

            regionView->shown().connect(std::bind(&Region::start, region.get()));

            addAllMedia(*region, *regionView, regionNode);

            view.addRegion(regionView, options.left, options.top, options.zindex);
            layout.addRegion(std::move(region));
        }
    }
}

void MainCompositor::addAllMedia(Region& region, RegionView& view, const xml_node& node)
{
    for(auto [nodeName, mediaNode] : node)
    {
        if(nodeName == ResourcesXlf::MediaNode)
        {
            addMedia(region, view, mediaNode);
        }
    }
}

void MainCompositor::addMedia(Region& region, RegionView& view, const xml_node& node)
{
    auto type = node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type));
    std::unique_ptr<Media> mediaModel;

    if(type != ResourcesXlf::AudioType)
    {
        auto factory = createVisibleMediaFactory(type, node);
        auto mediaViewInfo = factory->createView(view.width(), view.height());
        mediaModel = factory->createModel(mediaViewInfo.view);

        GetMediaPosition positionCalc{view.width(), view.height()};
        int left = positionCalc.getMediaLeft(mediaViewInfo.view->width(), mediaViewInfo.align);
        int top = positionCalc.getMediaTop(mediaViewInfo.view->height(), mediaViewInfo.valign);

        view.addMedia(mediaViewInfo.view, left, top);
    }
    else
    {
        auto factory = createMediaFactory(type, node);
        mediaModel = factory->createModel();
    }

    attachAdditionalMedia(*mediaModel, node);
    region.addMedia(std::move(mediaModel));
}

std::unique_ptr<VisibleMediaFactory> MainCompositor::createVisibleMediaFactory(const std::string& type, const xml_node& node)
{
    if(type == ResourcesXlf::ImageType)
        return std::make_unique<ImageFactory>(parse<ImageParser>(node));
    else if(type == ResourcesXlf::VideoType)
        return std::make_unique<VideoFactory>(parse<VideoParser>(node));
    else
        return std::make_unique<WebViewFactory>(parse<WebViewParser>(node));
}

std::unique_ptr<MediaFactory> MainCompositor::createMediaFactory(const std::string& type, const xml_node& node)
{
    if(type == ResourcesXlf::AudioType)
        return std::make_unique<AudioFactory>(parse<AudioParser>(node));
    else
        return nullptr;
}

void MainCompositor::attachAdditionalMedia(Media& media, const xml_node& node)
{
    for(auto [nodeName, attachedNode] : node)
    {
        auto factory = createMediaFactory(nodeName, attachedNode);
        if(factory)
        {
            media.attachMedia(factory->createModel());
        }
    }
}
