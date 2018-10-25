#pragma once

#include "constants.hpp"
#include "utils/Resources.hpp"

class IMainLayout;
class IMediaContainer;
class IMedia;
class IBackground;

struct MediaContainerWithPos;
struct MediaWithPos;

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
    std::vector<std::unique_ptr<IMedia> > collectInvisibleMedia(const xlf_node& containerNode);

    std::unique_ptr<IMedia> buildMedia(int containerWidth, int containerHeight, const xlf_node& mediaNode);
    std::unique_ptr<IMedia> buildImage(int containerWidth, int containerHeight, const ResourcesXlf::ImageOptions& opts);
    std::unique_ptr<IMedia> buildVideo(int containerWidth, int containerHeight, const ResourcesXlf::VideoOptions& opts);
    std::unique_ptr<IMedia> buildWebView(int containerWidth, int containerHeight, const ResourcesXlf::WebViewOptions& opts);

    std::unique_ptr<IMedia> buildMedia(const xlf_node& mediaNode);
    std::unique_ptr<IMedia> buildAudio(const ResourcesXlf::AudioOptions& opts);
};
