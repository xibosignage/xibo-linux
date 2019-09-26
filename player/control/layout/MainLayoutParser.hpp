#pragma once

#include "LayoutOptions.hpp"
#include "constants.hpp"

#include <boost/property_tree/ptree.hpp>

class IMainLayout;
class IOverlayLayout;
class IImage;

class MainLayoutParser
{
public:
    virtual ~MainLayoutParser() = default;
    std::unique_ptr<IMainLayout> layoutFrom(const ptree_node& node);

protected:
    virtual std::shared_ptr<IOverlayLayout> createView(const LayoutOptions& options);

private:
    LayoutOptions optionsFrom(const ptree_node& node);
    Uri uriFrom(const ptree_node& node);
    uint32_t colorFrom(const ptree_node& node);

    std::unique_ptr<IMainLayout> createLayout(const std::shared_ptr<IOverlayLayout>& view);
    std::shared_ptr<IImage> createBackground(const LayoutOptions& options);
    void addRegions(IMainLayout& layout, const ptree_node& node);
};
