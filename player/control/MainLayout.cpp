#include "MainLayout.hpp"
#include "utils/utilities.hpp"

#include "IRegion.hpp"
#include "IBackground.hpp"

#include "GtkOverlayWrapper.hpp"

MainLayout::MainLayout(int width, int height)
{
    m_handler = std::make_unique<GtkOverlayWrapper>();
    set_size(width, height);
}

MainLayout::MainLayout(int width, int height, std::shared_ptr<IOverlayWrapper> handler)
{
    m_handler = handler;
    set_size(width, height);
}

MainLayout::~MainLayout()
{
}

void MainLayout::add_region(std::unique_ptr<IRegion> region)
{
    if(region)
    {
        m_handler->add_child(region->handler(), region->top(), region->left(), region->width(), region->height());
        m_regions.push_back(std::move(region));
    }
}

IRegion& MainLayout::region(size_t index)
{
    return *m_regions.at(index);
}

size_t MainLayout::regions_count() const
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
    reorder_regions();
    if(m_background)
    {
        m_background->show();
    }
    for(auto&& region : m_regions)
    {
        region->show();
    }
}

void MainLayout::reorder_regions()
{
    std::sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
        return first->zindex() < second->zindex();
    });

    for(size_t i = 0; i != regions_count(); ++i)
    {
//        m_logger->trace("zindex: {} id: {} order: {}", m_regions[i]->zindex(), m_regions[i]->id(), i);
        m_handler->reorder_overlay(m_regions[i]->handler(), static_cast<int>(i));
    }
}

void MainLayout::set_background(std::unique_ptr<IBackground> background)
{
    // FIXME 100x100 layout and 1920x1080 background = crash
    if(background)
    {
        if(m_background)
        {
            m_handler->remove();
        }
        m_background = std::move(background);
        m_background->set_size(width(), height());
        m_handler->add(m_background->handler());
    }
}

IBackground& MainLayout::background()
{
    if(!m_background)
        throw std::runtime_error("No bacgkround set");
    return *m_background;
}

void MainLayout::set_size(int width, int height)
{
    m_handler->set_size(width, height); // NOTE TEST WITHOUT IT
    if(m_background)
    {
        m_background->set_size(width, height);
    }
    for(auto&& region : m_regions)
    {
        region->set_size(region->width(), region->height()); // scaling
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
