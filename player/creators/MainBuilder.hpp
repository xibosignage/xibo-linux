#pragma once

#include "constants.hpp"

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
    std::unique_ptr<IBackground> buildBackground(const xlf_node& layoutNode);
    std::vector<MediaContainerWithPos> collectContainers(const xlf_node& layoutNode);
    std::unique_ptr<IMediaContainer> buildContainer(const xlf_node& containerNode);
    std::vector<MediaWithPos> collectVisibleMedia(const xlf_node& containerNode);
    std::vector<std::unique_ptr<IMedia> > collectInvisibleMedia(const xlf_node& containerNode);
    std::unique_ptr<IMedia> buildMedia(const xlf_node& containerNode, const xlf_node& mediaNode);

};
