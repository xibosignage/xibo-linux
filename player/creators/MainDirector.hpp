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

struct RegionWithPos;
struct MediaWithPos;
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
    std::vector<MediaWithPos> collectVisibleMedia(int regionWidth, int regionHeight, const xlf_node& regionNode);
    std::vector<std::unique_ptr<IMedia>> collectInvisibleMedia(const xlf_node& regionNode);

    std::unique_ptr<IMedia> buildMedia(int regionWidth, int regionHeight, const xlf_node& mediaNode);
    std::unique_ptr<MediaBuilder> prepareImageBuilder(int regionWidth, int regionHeight, const ResourcesXlf::ImageOptions& opts);
    std::unique_ptr<MediaBuilder> prepareVideoBuilder(int regionWidth, int regionHeight, const ResourcesXlf::VideoOptions& opts);
    std::unique_ptr<MediaBuilder> prepareWebViewBuilder(int regionWidth, int regionHeight, const ResourcesXlf::WebViewOptions& opts);
    void attachAdditionalMedia(const xlf_node& mediaNode, MediaBuilder& builder);

    std::unique_ptr<IMedia> buildMedia(const xlf_node& mediaNode);
    std::unique_ptr<IMedia> buildAudio(const ResourcesXlf::AudioOptions& opts);
};
