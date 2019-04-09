#include "MainLayoutModel.hpp"

#include "parsers/Validators.hpp"

MainLayoutModel::MainLayoutModel(const LayoutOptions& options)
{
    m_width = options.width;
    m_height = options.height;
    m_backgroundUri = Validators::validateUri(options.backgroundUri);
    m_backgroundColor = Validators::validateColor(options.backgroundColor);
}

int MainLayoutModel::width() const
{
    return m_width;
}

int MainLayoutModel::height() const
{
    return m_height;
}

Uri MainLayoutModel::backgroundUri() const
{
    return m_backgroundUri;
}

uint32_t MainLayoutModel::backgroundColor() const
{
    return m_backgroundColor;
}

const Regions& MainLayoutModel::regions() const
{
    return m_regions;
}

void MainLayoutModel::addRegion(const std::shared_ptr<RegionModel>& region)
{
    m_regions.emplace_back(region);

    sortRegionsByZindex();
    assignZindexToRegions();
}

void MainLayoutModel::sortRegionsByZindex()
{
    std::stable_sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
        return first->zindex() < second->zindex();
    });
}

void MainLayoutModel::assignZindexToRegions()
{
    for(size_t i = 0; i !=  m_regions.size(); ++i)
    {
        m_regions[i]->setZindex(static_cast<int>(i));
    }
}
