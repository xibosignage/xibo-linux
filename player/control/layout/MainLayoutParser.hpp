#pragma once

#include "constants.hpp"
#include "LayoutOptions.hpp"

#include <boost/property_tree/ptree.hpp>

class IMainLayout;
class IOverlayLayout;
class IImage;

class MainLayoutParser
{
public:
    virtual ~MainLayoutParser() = default;
    std::unique_ptr<IMainLayout> layoutFrom(const xml_node& node);

protected:
    virtual std::shared_ptr<IOverlayLayout> createView(const LayoutOptions& options);

private:
    LayoutOptions optionsFrom(const xml_node& node);
    Uri uriFrom(const xml_node& node);
    uint32_t colorFrom(const xml_node& node);

    std::unique_ptr<IMainLayout> createLayout(const std::shared_ptr<IOverlayLayout>& view);
    std::shared_ptr<IImage> createBackground(const LayoutOptions& options);
    void addRegions(IMainLayout& layout, const xml_node& node);
};

