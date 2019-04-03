#pragma once

#include "constants.hpp"

#include "control/layout/MainLayout.hpp"
#include "utils/FilePath.hpp"

class MainLayoutView;
class RegionView;
class Image;
class WebView;
class ImageOptions;
class AudioOptions;
class MediaPlayerOptions;
class WebViewOptions;

class MainParser
{
public:
    std::pair<std::unique_ptr<MainLayout>, std::shared_ptr<MainLayoutView>> parseLayout(int layoutId);

private:
    FilePath getXlfFilePath(int layoutId);

    std::pair<std::unique_ptr<MainLayout>, std::shared_ptr<MainLayoutView>> parseLayout(const xml_node& node);
    void addRegions(MainLayout& layout, MainLayoutView& view, const xml_node& node);
    void addAllMedia(Region& region, RegionView& view, const xml_node& node);
    void addMedia(Region& region, RegionView& view, const xml_node& node);
    void attachAdditionalMedia(Media& media, const xml_node& node);

private:
    std::shared_ptr<Image> createBackground(const MainLayoutOptions& options);

    void addMedia(Region& region, RegionView& view, const ImageOptions& options);
    std::shared_ptr<Image> create(int width, int height, const ImageOptions& options);
    void addMedia(Region& region, RegionView& view, const WebViewOptions& options);
    std::shared_ptr<WebView> create(int width, int height, const WebViewOptions& options);
    void addAudio(Region& region, RegionView& view, const MediaPlayerOptions& options);
    void addVideo(Region& region, RegionView& view, const MediaPlayerOptions& options);

    template<typename Parser>
    auto parse(const xml_node& node)
    {
        Parser parser{node};
        return parser.parse();
    }

};
