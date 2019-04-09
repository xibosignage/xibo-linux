#pragma once

#include "constants.hpp"

#include "control/layout/MainLayout.hpp"
#include "utils/FilePath.hpp"

class MainLayoutView;
class RegionView;
class Widget;
class Controller;
class VisibleMediaFactory;
class MediaFactory;

class MainCompositor
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
    std::unique_ptr<VisibleMediaFactory> createVisibleMediaFactory(const std::string& type, const xml_node& node);
    std::unique_ptr<MediaFactory> createMediaFactory(const std::string& type, const xml_node& node);

private:
    std::shared_ptr<Widget> createBackground(const MainLayoutOptions& options);

    template<typename Parser>
    auto parse(const xml_node& node)
    {
        Parser parser{node};
        return parser.parse();
    }

};
