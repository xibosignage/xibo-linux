#pragma once

#include "constants.hpp"

#include "parsers/LayoutOptions.hpp"
#include "parsers/RegionOptions.hpp"
#include "parsers/ImageOptions.hpp"
#include "parsers/WebViewOptions.hpp"
#include "parsers/VideoOptions.hpp"
#include "parsers/AudioOptions.hpp"

class IMainLayout;
class IMediaContainer;
class IMedia;
class IBackground;

struct MediaContainerWithPos;
struct MediaWithPos;
class MediaBuilder;

class MainBuilder
{

public:
    MainBuilder() = default;
    std::unique_ptr<IMainLayout> buildLayoutWithChildren(const xlf_node& tree);

private:
    std::unique_ptr<IMainLayout> buildLayout(const xlf_node& layoutNode);
    std::unique_ptr<IBackground> buildBackground(const ResourcesXlf::LayoutOptions& opts);
    std::vector<MediaContainerWithPos> collectContainers(const xlf_node& layoutNode);
    std::unique_ptr<IMediaContainer> buildContainer(const xlf_node& containerNode);
    std::vector<MediaWithPos> collectVisibleMedia(int containerWidth, int containerHeight, const xlf_node& containerNode);
    std::vector<std::unique_ptr<IMedia>> collectInvisibleMedia(const xlf_node& containerNode);

    std::unique_ptr<IMedia> buildMedia(int containerWidth, int containerHeight, const xlf_node& mediaNode);
    std::unique_ptr<MediaBuilder> prepareImageBuilder(int containerWidth, int containerHeight, const ResourcesXlf::ImageOptions& opts);
    std::unique_ptr<MediaBuilder> prepareVideoBuilder(int containerWidth, int containerHeight, const ResourcesXlf::VideoOptions& opts);
    std::unique_ptr<MediaBuilder> prepareWebViewBuilder(int containerWidth, int containerHeight, const ResourcesXlf::WebViewOptions& opts);
    void attachAdditionalMedia(const xlf_node& mediaNode, MediaBuilder& builder);

    std::unique_ptr<IMedia> buildMedia(const xlf_node& mediaNode);
    std::unique_ptr<IMedia> buildAudio(const ResourcesXlf::AudioOptions& opts);
};
