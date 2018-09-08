#include "MainLayout.hpp"

#include "IMediaContainer.hpp"
#include "IBackground.hpp"

#include "utils/utilities.hpp"
#include "adaptors/GtkOverlayAdaptor.hpp"
#include "adaptors/IImageAdaptor.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"

MainLayout::MainLayout(int width, int height) :
    MainLayout(width, height, std::make_unique<GtkOverlayAdaptor>())
{
}

MainLayout::MainLayout(int width, int height, std::unique_ptr<IOverlayAdaptor> handler) :
    m_handler(std::move(handler))
{
    setSize(width, height);
}

MainLayout::~MainLayout()
{
}

void MainLayout::addMediaContainer(std::unique_ptr<IMediaContainer> mediaContainer, int x, int y)
{
    if(mediaContainer)
    {
        if(mediaContainer->width() > width() || mediaContainer->height() > height())
            throw std::runtime_error("Region width/height should not be greater than in layout");

        m_handler->addChild(mediaContainer->handler(),
                            mediaContainer->width(), mediaContainer->height(),
                            x, y);
        m_containers.push_back(std::move(mediaContainer));
    }
}

void MainLayout::removeAllContainers()
{
    m_handler->removeChildren();
    m_containers.clear();
}

IOverlayAdaptor& MainLayout::handler()
{
    return *m_handler;
}

void MainLayout::show()
{
    m_handler->show();
    showBackground();
    sortReorderAndShowContainers();
}

void MainLayout::sortAndReorderContainers()
{
    sortContainersByZorder();

    for(size_t i = 0; i != m_containers.size(); ++i)
    {
        int orderInParentWidget = static_cast<int>(i);
        utils::logger()->trace("Zindex: {} Order: {}", m_containers[i]->zorder(),
                                                       orderInParentWidget);
        m_handler->reorderChild(m_containers[i]->handler(), orderInParentWidget);
    }
}

void MainLayout::sortContainersByZorder()
{
    std::sort(m_containers.begin(), m_containers.end(), [=](const auto& first, const auto& second){
        return first->zorder() < second->zorder();
    });
}

void MainLayout::showBackground()
{
    if(m_background)
    {
        m_background->show();
    }
}

void MainLayout::sortReorderAndShowContainers()
{
    sortAndReorderContainers();
    for(auto&& container : m_containers)
    {
        container->show();
    }
}

void MainLayout::removePreviousBackground()
{
    if(m_background)
    {
        m_handler->removeMainChild();
    }
}

void MainLayout::setBackgroundSize(int width, int height)
{
    if(m_background)
    {
        m_background->setSize(width, height);
    }
}

// NOTE check 100x100 layout and 1920x1080 background = crash
void MainLayout::setBackground(std::unique_ptr<IBackground> background)
{
    if(background)
    {
        if(background->width() != width() || background->height() != height())
            throw std::runtime_error("Background's and layout's size should be the same");

        removePreviousBackground();
        m_background = std::move(background);
        m_handler->addMainChild(m_background->handler());
    }
}

IBackground& MainLayout::background()
{
    if(!m_background)
        throw std::runtime_error("No background set");

    return *m_background;
}

void MainLayout::setSize(int width, int height)
{
    if(width < MIN_DISPLAY_WIDTH || width > MAX_DISPLAY_WIDTH || height < MIN_DISPLAY_HEIGHT || height > MAX_DISPLAY_HEIGHT)
        throw std::runtime_error("Width or height is too small/large");

    m_handler->setSize(width, height);
    setBackgroundSize(width, height);
}

int MainLayout::width() const
{
    return m_handler->width();
}

int MainLayout::height() const
{
    return m_handler->height();
}
