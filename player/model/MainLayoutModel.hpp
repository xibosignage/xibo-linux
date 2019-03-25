#pragma once

#include "utils/Uri.hpp"
#include "model/RegionModel.hpp"
#include "parsers/Options.hpp"

#include <memory>

using Regions = std::vector<std::shared_ptr<RegionModel>>;

class MainLayoutModel
{
public:
    MainLayoutModel(const LayoutOptions& options);

    const Regions& regions() const;
    void addRegion(const std::shared_ptr<RegionModel>& region);

    int width() const;
    int height() const;

    Uri backgroundUri() const;
    uint32_t backgroundColor() const;

private:
    void sortRegionsByZindex();

private:
    int m_width;
    int m_height;
    Uri m_backgroundUri;
    uint32_t m_backgroundColor;
    Regions m_regions;

};