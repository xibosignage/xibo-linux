#pragma once

#include "constants.hpp"

#include <filesystem>

class IMainLayout;
class IMediaContainer;
class IMedia;

class MainBuilder
{

public:
    MainBuilder() = default;
    std::unique_ptr<IMainLayout> buildLayoutWithChildren(const xlf_node& tree);

private:
    boost::property_tree::ptree parse(const std::filesystem::path& xlfPath);

    std::unique_ptr<IMainLayout> buildLayout(const xlf_node& layoutNode);
    std::unique_ptr<IMediaContainer> createContainer(const xlf_node& containerNode);

    void createAndAddMediaToContainer(IMediaContainer& mediaController, const xlf_node& mediaNode);
    std::unique_ptr<IMedia> createVisibleMedia(const std::string& type, const xlf_node& mediaNode);
    std::unique_ptr<IMedia> createInvisibleMedia(const std::string& type, const xlf_node& mediaNode);

};
