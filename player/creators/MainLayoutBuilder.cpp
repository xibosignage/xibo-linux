#include "MainLayoutBuilder.hpp"
#include "constants.hpp"

#include "control/MainLayout.hpp"
#include "control/IBackground.hpp"
#include "control/IRegion.hpp"

#include "adaptors/GtkOverlayAdaptor.hpp"

#include <cassert>

MainLayoutBuilder::MainLayoutBuilder()
{
}

std::unique_ptr<IMainLayout> MainLayoutBuilder::build()
{
    auto layout = createLayout();
    prepareLayout(*layout);
    return layout;
}

MainLayoutBuilder& MainLayoutBuilder::options(const ResourcesXlf::LayoutOptions& opts)
{
    m_schemeVersion = opts.schemaVersion();
    m_width = getWidthOption(opts.width());
    m_height = getHeightOption(opts.height());
    return *this;
}

std::unique_ptr<IMainLayout> MainLayoutBuilder::createLayout()
{
    assert(m_adaptor);

    return std::unique_ptr<MainLayout>(new MainLayout{m_width, m_height, std::move(m_adaptor)});
}

std::unique_ptr<IOverlayAdaptor> MainLayoutBuilder::createAdaptor()
{
    if(!m_adaptor) return std::make_unique<GtkOverlayAdaptor>();

    return std::move(m_adaptor);
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

MainLayoutBuilder& MainLayoutBuilder::adaptor(std::unique_ptr<IOverlayAdaptor>&& adaptor)
{
    m_adaptor = std::move(adaptor);
    return *this;
}

int MainLayoutBuilder::getWidthOption(int width)
{
    checkWidth(width);

    return width;
}

void MainLayoutBuilder::checkWidth(int width)
{
    if(width < MIN_DISPLAY_WIDTH)
        throw std::invalid_argument("Width or height is too small");
}

int MainLayoutBuilder::getHeightOption(int height)
{
    checkHeight(height);

    return height;
}

void MainLayoutBuilder::checkHeight(int height)
{
    if(height < MIN_DISPLAY_HEIGHT)
        throw std::invalid_argument("Width or height is too small");
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
