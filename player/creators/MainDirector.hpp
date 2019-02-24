#pragma once

#include "constants.hpp"

class BackgroundOptions;
class ImageOptions;
class VideoOptions;
class AudioOptions;
class WebViewOptions;

class IMainLayout;
class IRegion;
class IMedia;
class IBackground;
class IRegionContent;
class Image;
class Video;
class Audio;
class WebView;

struct RegionWithPos;
struct ContentWithPos;

class MainDirector
{

public:
    MainDirector() = default;
    std::unique_ptr<IMainLayout> buildLayoutWithChildren(int layoutId);

private:
    std::unique_ptr<IMainLayout> buildLayout(const xml_node& layoutNode);
    std::string getLayoutXlfPath(int layoutId);
    std::unique_ptr<IBackground> buildBackground(const BackgroundOptions& opts);
    std::vector<RegionWithPos> collectRegions(const xml_node& layoutNode);
    std::unique_ptr<IRegion> buildRegion(const xml_node& regionNode);
    std::vector<ContentWithPos> collectContent(int regionWidth, int regionHeight, const xml_node& regionNode);
    std::vector<std::unique_ptr<IMedia>> collectInvisibleMedia(const xml_node& regionNode);

    std::unique_ptr<IMedia> buildMedia(int regionWidth, int regionHeight, const xml_node& mediaNode);
    std::unique_ptr<Image> buildImage(int regionWidth, int regionHeight, const ImageOptions& opts);
    std::unique_ptr<Video> buildVideo(int regionWidth, int regionHeight, const VideoOptions& opts);
    std::unique_ptr<Audio> buildAudio(const AudioOptions& opts);
    std::unique_ptr<WebView> buildWebView(int regionWidth, int regionHeight, const WebViewOptions& opts);
    void attachAdditionalMedia(const xml_node& mediaNode, IRegionContent& content);


};
