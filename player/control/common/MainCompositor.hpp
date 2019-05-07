#pragma once

#include "constants.hpp"

#include "control/layout/MainLayout.hpp"
#include "common/FilePath.hpp"

class MainLayoutView;
class RegionView;
class Widget;
class Controller;
class MediaFactory;

class MainCompositor
{
public:
    std::unique_ptr<MainLayout> parseLayout(int layoutId);

private:
    FilePath getXlfFilePath(int layoutId);

    std::unique_ptr<MainLayout> parseLayout(const xml_node& node);
    void addRegions(MainLayout& layout, const xml_node& node);
    void addAllMedia(Region& region, const xml_node& node);
    void addMedia(Region& region, const xml_node& node);
    void attachAdditionalMedia(Media& media, const xml_node& node);
    std::unique_ptr<MediaFactory> createMediaFactory(const std::string& type, int width, int height, const xml_node& node);

private:
    std::shared_ptr<MainLayoutView> createLayoutView(const MainLayoutOptions& options);
    std::shared_ptr<Widget> createBackground(const MainLayoutOptions& options);

    template<typename Parser>
    auto parse(const xml_node& node)
    {
        Parser parser{node};
        return parser.parse();
    }

};
