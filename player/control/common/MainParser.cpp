#include "MainParser.hpp"

#include "XlfResources.hpp"

#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"

#include "control/layout/MainLayoutParser.hpp"
#include "control/layout/MainLayoutView.hpp"

#include "control/region/RegionParser.hpp"
#include "control/region/RegionView.hpp"

#include "control/common/Image.hpp"
#include "control/media/image/ImageParser.hpp"
#include "control/media/GetMediaPosition.hpp"
#include "control/media/MediaResources.hpp"

#include "control/media/webview/WebViewParser.hpp"
#include "control/media/webview/WebView.hpp"

#include "control/media/player/AudioParser.hpp"
#include "control/media/player/VideoParser.hpp"
#include "control/media/player/VideoWindow.hpp"
#include "control/media/player/MediaPlayer.hpp"
#include "control/media/player/MediaPlayerController.hpp"

std::pair<std::unique_ptr<MainLayout>, std::shared_ptr<MainLayoutView>> MainParser::parseLayout(int layoutId)
{
    auto tree = Utils::parseXmlFromPath(getXlfFilePath(layoutId));
    auto layoutNode = tree.get_child(ResourcesXlf::LayoutNode);

    return parseLayout(layoutNode);
}

FilePath MainParser::getXlfFilePath(int layoutId)
{
    auto xlfFile = std::to_string(layoutId) + ".xlf";
    return Resources::directory() / xlfFile;
}

std::pair<std::unique_ptr<MainLayout>, std::shared_ptr<MainLayoutView>> MainParser::parseLayout(const xml_node& layoutNode)
{
    auto options = MainLayoutParser{layoutNode}.parse();
    auto layout = std::make_unique<MainLayout>();
    auto layoutView = std::make_shared<MainLayoutView>(options.width, options.height);

    layoutView->addBackground(createBackground(options));
    addRegions(*layout, *layoutView, layoutNode);

    return std::pair{std::move(layout), layoutView};
}

std::shared_ptr<Image> MainParser::createBackground(const MainLayoutOptions& options)
{
    auto background = std::make_shared<Image>(options.width, options.height);

    if(options.backgroundUri.isValid())
        background->loadFromFile(options.backgroundUri, true);
    else
        background->setColor(options.backgroundColor);

    return background;
}

void MainParser::addRegions(MainLayout& layout, MainLayoutView& view, const xml_node& node)
{
    for(auto [nodeName, regionNode] : node)
    {
        if(nodeName == ResourcesXlf::RegionNode)
        {
            auto options = RegionParser{regionNode}.parse();
            auto region = std::make_unique<Region>(options.id, options.looped);
            auto regionView = std::make_shared<RegionView>(options.width, options.height);

            regionView->shown().connect(std::bind(&Region::start, region.get()));

            addAllMedia(*region, *regionView, regionNode);

            view.addRegion(regionView, options.left, options.top, options.zindex);
            layout.addRegion(std::move(region));
        }
    }
}

void MainParser::addAllMedia(Region& region, RegionView& view, const xml_node& node)
{
    for(auto [nodeName, mediaNode] : node)
    {
        if(nodeName == ResourcesXlf::MediaNode)
        {
            auto options = MediaParser{mediaNode}.baseOptions();
            auto media = std::make_unique<Media>(options);

            addMedia(region, view, mediaNode);
            attachAdditionalMedia(*media, mediaNode);
        }
    }
}

void MainParser::addMedia(Region& region, RegionView& view, const xml_node& node)
{
    auto type = node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Media::Type));

    if(type == ResourcesXlf::ImageType)
        addMedia(region, view, parse<ImageParser>(node));
    else if(type == ResourcesXlf::AudioType)
        addAudio(region, view, parse<AudioParser>(node));
    else if(type == ResourcesXlf::VideoType)
        addVideo(region, view, parse<VideoParser>(node));
    else
        addMedia(region, view, parse<WebViewParser>(node));
}

void MainParser::attachAdditionalMedia(Media& media, const xml_node& node)
{
    for(auto [nodeName, attachedNode] : node)
    {
        if(nodeName == ResourcesXlf::AudioType)
        {
            auto options = parse<AudioParser>(attachedNode);
//            media.attachMedia(std::move(model));
        }
    }
}

void MainParser::addMedia(Region& region, RegionView& view, const ImageOptions& options)
{
    auto model = std::make_unique<Media>(options);
    auto image = create(view.width(), view.height(), options);

    GetMediaPosition positionCalc{view.width(), view.height()};
    int left = positionCalc.getMediaLeft(image->width(), options.align);
    int top = positionCalc.getMediaTop(image->height(), options.valign);

    model->started().connect(std::bind(&Image::show, image.get()));
    model->stopped().connect(std::bind(&Image::hide, image.get()));

    region.addMedia(std::move(model));
    view.addMedia(image, left, top);
}

std::shared_ptr<Image> MainParser::create(int width, int height, const ImageOptions& options)
{
    auto image = std::make_shared<Image>(width, height);
    image->loadFromFile(options.uri, options.scaleType == MediaGeometry::ScaleType::Scaled ? true : false);
    return image;
}

void MainParser::addMedia(Region& region, RegionView& view, const WebViewOptions& options)
{
    auto model = std::make_unique<Media>(options);
    auto webview = create(view.width(), view.height(), options);

    model->started().connect(std::bind(&WebView::show, webview.get()));
    model->stopped().connect(std::bind(&WebView::hide, webview.get()));

    region.addMedia(std::move(model));
    view.addMedia(webview, DEFAULT_XPOS, DEFAULT_YPOS);
}

std::shared_ptr<WebView> MainParser::create(int width, int height, const WebViewOptions& options)
{
    auto webview = std::make_shared<WebView>(width, height);
    webview->load(options.uri);
    if(options.transparency == WebViewOptions::Transparency::Enable)
    {
        webview->enableTransparency();
    }
    return webview;
}

void MainParser::addAudio(Region& region, RegionView&, const MediaPlayerOptions& options)
{
    auto model = std::make_unique<Media>(options);
    new MediaPlayerController(options, *model, std::make_unique<MediaPlayer>());
    region.addMedia(std::move(model));
}

void MainParser::addVideo(Region& region, RegionView& view, const MediaPlayerOptions& options)
{
    auto videoPlayer = std::make_unique<MediaPlayer>();
    auto videoWindow = std::make_shared<VideoWindow>(view.width(), view.height());
    videoPlayer->setOutputWindow(videoWindow);
    auto model = std::make_unique<Media>(options);
    new MediaPlayerController(options, *model, std::move(videoPlayer));

    region.addMedia(std::move(model));
    view.addMedia(videoWindow, DEFAULT_XPOS, DEFAULT_YPOS);
}
