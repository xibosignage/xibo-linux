#pragma once

#include "constants.hpp"

#include "parsers/LayoutOptions.hpp"
#include "parsers/RegionOptions.hpp"
#include "parsers/ImageOptions.hpp"
#include "parsers/WebViewOptions.hpp"
#include "parsers/VideoOptions.hpp"
#include "parsers/AudioOptions.hpp"

class IMainLayout;
class IRegion;
class IMedia;
class IBackground;
class IRegionContent;

struct RegionWithPos;
struct ContentWithPos;
class MediaBuilder;

class Image;
class Video;
class Audio;
class WebView;

class MainDirector
{

public:
    MainDirector() = default;
    std::unique_ptr<IMainLayout> buildLayoutWithChildren(int layoutId);

private:
    std::unique_ptr<IMainLayout> buildLayout(const xml_node& layoutNode);
    std::string getLayoutXlfPath(int layoutId);
    std::unique_ptr<IBackground> buildBackground(const ResourcesXlf::LayoutOptions& opts);
    std::vector<RegionWithPos> collectRegions(const xml_node& layoutNode);
    std::unique_ptr<IRegion> buildRegion(const xml_node& regionNode);
    std::vector<ContentWithPos> collectContent(int regionWidth, int regionHeight, const xml_node& regionNode);
    std::vector<std::unique_ptr<IMedia>> collectInvisibleMedia(const xml_node& regionNode);

    std::unique_ptr<IMedia> buildMedia(int regionWidth, int regionHeight, const xml_node& mediaNode);
    std::unique_ptr<Image> buildImage(int regionWidth, int regionHeight, const ResourcesXlf::ImageOptions& opts);
    std::unique_ptr<Video> buildVideo(int regionWidth, int regionHeight, const ResourcesXlf::VideoOptions& opts);
    std::unique_ptr<Audio> buildAudio(const ResourcesXlf::AudioOptions& opts);
    std::unique_ptr<WebView> buildWebView(int regionWidth, int regionHeight, const ResourcesXlf::WebViewOptions& opts);
    void attachAdditionalMedia(const xml_node& mediaNode, IRegionContent& content);


};
