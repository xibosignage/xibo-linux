#include "MainLayoutBuilder.hpp"
#include "constants.hpp"

#include "control/MainLayout.hpp"
#include "control/IBackground.hpp"
#include "control/IRegion.hpp"
#include "adaptors/GtkOverlayAdaptor.hpp"

#include <cassert>

std::unique_ptr<IMainLayout> MainLayoutBuilder::build()
{
    auto layout = createLayout();
    prepareLayout(*layout);
    return layout;
}

std::unique_ptr<IMainLayout> MainLayoutBuilder::createLayout()
{
    return std::make_unique<MainLayout>(m_width, m_height, createAdaptor());
}

std::unique_ptr<IOverlayAdaptor> MainLayoutBuilder::createAdaptor()
{
    return std::make_unique<GtkOverlayAdaptor>();
}

MainLayoutBuilder& MainLayoutBuilder::width(int width)
{
    checkWidth(width);

    m_width = width;
    return *this;
}

void MainLayoutBuilder::checkWidth(int width)
{
    if(width < MIN_DISPLAY_WIDTH)
        throw std::invalid_argument("Width or height is too small");
}

MainLayoutBuilder& MainLayoutBuilder::height(int height)
{
    checkHeight(height);

    m_height = height;
    return *this;
}

void MainLayoutBuilder::checkHeight(int height)
{
    if(height < MIN_DISPLAY_HEIGHT)
        throw std::invalid_argument("Width or height is too small");
}

MainLayoutBuilder& MainLayoutBuilder::background(std::unique_ptr<IBackground>&& background)
{
    m_background = std::move(background);
    return *this;
}

MainLayoutBuilder& MainLayoutBuilder::regions(std::vector<RegionWithPos>&& regions)
{
    m_regions = std::move(regions);
    return *this;
}

void MainLayoutBuilder::prepareLayout(IMainLayout& layout)
{
    assert(m_background);
    checkRegionsCount(m_regions.size());

    layout.setBackground(std::move(m_background));

    for(auto&& ct : m_regions)
    {
        layout.addRegion(std::move(ct.region), ct.x, ct.y);
    }
}

void MainLayoutBuilder::checkRegionsCount(size_t regionsCount)
{
    if(regionsCount == 0)
        throw std::runtime_error("You should add at least 1 region");
}
