#pragma once

#include "constants.hpp"

class IMainLayout;
class IMediaContainer;
class IMedia;
class IBackground;

struct MediaContainerStruct;
struct MediaStruct;

class MainBuilder
{

public:
    MainBuilder() = default;
    std::unique_ptr<IMainLayout> buildLayoutWithChildren(const xlf_node& tree);

private:
    std::unique_ptr<IMainLayout> buildLayout(const xlf_node& layoutNode);
    std::unique_ptr<IBackground> buildBackground(const xlf_node& layoutNode);
    std::vector<MediaContainerStruct> collectContainers(const xlf_node& layoutNode);
    std::unique_ptr<IMediaContainer> buildContainer(const xlf_node& containerNode);
    std::vector<MediaStruct> collectMedia(const xlf_node& containerNode);
    std::unique_ptr<IMedia> buildMedia(const xlf_node& containerNode, const xlf_node& mediaNode);

};
