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

class MainDirector
{

public:
    MainDirector() = default;
    std::unique_ptr<IMainLayout> buildLayoutWithChildren(const xlf_node& tree);

private:
    std::unique_ptr<IMainLayout> buildLayout(const xlf_node& layoutNode);
    std::unique_ptr<IBackground> buildBackground(const ResourcesXlf::LayoutOptions& opts);
    std::vector<RegionWithPos> collectRegions(const xlf_node& layoutNode);
    std::unique_ptr<IRegion> buildRegion(const xlf_node& regionNode);
    std::vector<ContentWithPos> collectContent(int regionWidth, int regionHeight, const xlf_node& regionNode);
    std::vector<std::unique_ptr<IMedia>> collectInvisibleMedia(const xlf_node& regionNode);

    std::unique_ptr<IMedia> buildMedia(int regionWidth, int regionHeight, const xlf_node& mediaNode);
    std::unique_ptr<IMedia> buildImage(int regionWidth, int regionHeight, const ResourcesXlf::ImageOptions& opts);
    std::unique_ptr<IMedia> buildVideo(int regionWidth, int regionHeight, const ResourcesXlf::VideoOptions& opts);
    std::unique_ptr<IMedia> buildAudio(const ResourcesXlf::AudioOptions& opts);
    std::unique_ptr<IMedia> buildWebView(int regionWidth, int regionHeight, const ResourcesXlf::WebViewOptions& opts);
    void attachAdditionalMedia(const xlf_node& mediaNode, IRegionContent& content);


};
