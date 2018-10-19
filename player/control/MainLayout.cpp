#include "MainLayout.hpp"

#include "IMediaContainer.hpp"
#include "IBackground.hpp"

#include "adaptors/IImageAdaptor.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"
#include "adaptors/IOverlayAdaptor.hpp"
#include "utils/Utilities.hpp"

#include <cassert>

MainLayout::MainLayout(std::unique_ptr<IOverlayAdaptor>&& handler) :
    m_handler(std::move(handler))
{
    assert(m_handler);
}

MainLayout::~MainLayout()
{
}

int MainLayout::width() const
{
    return m_handler->width();
}

int MainLayout::height() const
{
    return m_handler->height();
}

void MainLayout::scale(double scaleX, double scaleY)
{
    assert(m_background);
    assert(m_containers.size() > 0);

    m_handler->scale(scaleX, scaleY);
    m_background->scale(scaleX, scaleY);
    scaleContainers(scaleX, scaleY);
}

void MainLayout::scaleContainers(double scaleX, double scaleY)
{
    for(auto&& container : m_containers)
    {
        container->scale(scaleX, scaleY);
    }
}

void MainLayout::addMediaContainer(std::unique_ptr<IMediaContainer>&& mediaContainer, int x, int y)
{
    assert(mediaContainer);

    checkContainerSize(mediaContainer->width(), mediaContainer->height());
    checkContainerCoordinates(x, y);

    m_handler->addChild(mediaContainer->handler(), mediaContainer->width(), mediaContainer->height(), x, y);
    m_containers.push_back(std::move(mediaContainer));
}

void MainLayout::checkContainerSize(int containerWidth, int containerHeight)
{
    if(containerWidth > width() || containerHeight > height())
        throw std::invalid_argument("Container width/height should not be greater than in layout");
}

void MainLayout::checkContainerCoordinates(int x, int y)
{
    if(x > width() || x < MIN_X_POS || y > height() || y < MIN_Y_POS)
        throw std::invalid_argument("Container x/y pos should not be greater than layout's width and height");
}

IOverlayAdaptor& MainLayout::handler()
{
    return *m_handler;
}

void MainLayout::show()
{
    assert(m_background);
    assert(m_containers.size() > 0);

    if(!m_handler->isShown())
    {
        m_handler->show();
        m_background->show();
        sortReorderAndShowContainers();
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

void MainLayout::sortAndReorderContainers()
{
    sortContainersByZorder();

    for(size_t i = 0; i != m_containers.size(); ++i)
    {
        int orderInParentWidget = static_cast<int>(i);
        m_handler->reorderChild(m_containers[i]->handler(), orderInParentWidget);

        Utils::logger()->trace("Zorder: {} Order in overlay: {}", m_containers[i]->zorder(), orderInParentWidget);
    }
}

void MainLayout::sortContainersByZorder()
{
    std::sort(m_containers.begin(), m_containers.end(), [=](const auto& first, const auto& second){
        return first->zorder() < second->zorder();
    });
}

void MainLayout::setBackground(std::unique_ptr<IBackground>&& background)
{
    assert(background);

    checkBackgroundSize(background->width(), background->height());

    removePreviousBackgroundIfSet();
    m_background = std::move(background);
    m_handler->addMainChild(m_background->handler());
}

void MainLayout::checkBackgroundSize(int backgroundWidth, int backgroundHeight)
{
    if(backgroundWidth != width() || backgroundHeight != height())
        throw std::invalid_argument("Background's and layout's size should be the same");
}

void MainLayout::removePreviousBackgroundIfSet()
{
    if(m_background)
    {
        m_handler->removeMainChild();
    }
}
