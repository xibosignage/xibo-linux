#include "MainLayout.hpp"
#include "utils/utilities.hpp"

#include "IRegion.hpp"
#include "Background.hpp"

#include "GtkOverlayWrapper.hpp"

const int MIN_WIDTH = 160;
const int MAX_WIDTH = 9999;
const int MIN_HEIGHT = 120;
const int MAX_HEIGHT = 9999;

MainLayout::MainLayout(int width, int height) :
    MainLayout(width, height, std::make_shared<GtkOverlayWrapper>())
{
}

MainLayout::MainLayout(int width, int height, std::shared_ptr<IOverlayWrapper> handler)
{
    m_handler = handler;
    setSize(width, height);
}

MainLayout::~MainLayout()
{
}

void MainLayout::addRegion(std::shared_ptr<IRegion> region)
{
    if(region)
    {
        if(region->width() > width() || region->height() > height())
            throw std::runtime_error("Region width/height should not be greater than in layout");

        m_handler->addChild(region->handler(), region->top(), region->left(), region->width(), region->height());
        m_regions.push_back(std::move(region));
    }
}

void MainLayout::removeAllRegions()
{
    m_handler->removeChildren();
    m_regions.clear();
}

IRegion& MainLayout::region(size_t index)
{
    return *m_regions.at(index);
}

size_t MainLayout::regionsCount() const
{
    return m_regions.size();
}

IOverlayWrapper& MainLayout::handler()
{
    return *m_handler;
}

void MainLayout::show()
{
    m_handler->show();
    reorderRegions();
    if(m_background)
    {
        m_background->show();
    }
    for(auto&& region : m_regions)
    {
        region->show();
    }
}

void MainLayout::reorderRegions()
{
    std::sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
        return first->zindex() < second->zindex();
    });

    for(size_t i = 0; i != regionsCount(); ++i)
    {
        utils::logger()->trace("zindex: {} id: {} order: {}", m_regions[i]->zindex(),
                                                              m_regions[i]->id(), i);
        m_handler->reorderChild(m_regions[i]->handler(), static_cast<int>(i));
    }
}

void MainLayout::setBackground(std::shared_ptr<IBackground> background)
{
    // FIXME 100x100 layout and 1920x1080 background = crash
    if(background)
    {
        if(background->width() != width() || background->height() != height())
            throw std::runtime_error("Background's and layout's size should be the same");

        if(m_background)
        {
            m_handler->removeMainChild();
        }
        m_background = std::move(background);
        m_handler->addMainChild(m_background->handler());
    }
}

void MainLayout::setSize(int width, int height)
{
    if(width < MIN_WIDTH || width > MAX_WIDTH || height < MIN_HEIGHT || height > MAX_HEIGHT)
        throw std::runtime_error("Width or height is too small/large");

    m_handler->setSize(width, height);
    if(m_background)
    {
        m_background->setSize(width, height);
    }
}

int MainLayout::width() const
{
    return m_handler->width();
}

int MainLayout::height() const
{
    return m_handler->height();
}
