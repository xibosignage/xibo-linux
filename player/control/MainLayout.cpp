#include "MainLayout.hpp"

#include "IMediaContainer.hpp"
#include "IBackground.hpp"

#include "adaptors/IImageAdaptor.hpp"
#include "adaptors/IFixedLayoutAdaptor.hpp"
#include "adaptors/IOverlayAdaptor.hpp"

#include "utils/utilities.hpp"

MainLayout::MainLayout(std::unique_ptr<IOverlayAdaptor>&& handler) :
    m_handler(std::move(handler))
{
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

void MainLayout::setSize(int width, int height)
{
    checkLayoutNewSize(width, height);

    m_handler->setSize(width, height);
    setBackgroundSize(width, height);
}

void MainLayout::setBackgroundSize(int width, int height)
{
    if(m_background)
    {
        m_background->setSize(width, height);
    }
}

void MainLayout::checkLayoutNewSize(int width, int height)
{
    if(width < MIN_DISPLAY_WIDTH || width > MAX_DISPLAY_WIDTH || height < MIN_DISPLAY_HEIGHT || height > MAX_DISPLAY_HEIGHT)
        throw std::invalid_argument("Width or height is too small/large");
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

// TEST coordinates
void MainLayout::checkContainerCoordinates(int x, int y)
{
    if(x > width() || x < MIN_X_POS || y > height() || y < MIN_Y_POS)
        throw std::invalid_argument("Container x/y pos should not be greater than layout's width and height");
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

void MainLayout::sortAndReorderContainers()
{
    sortContainersByZorder();

    for(size_t i = 0; i != m_containers.size(); ++i)
    {
        int orderInParentWidget = static_cast<int>(i);
        m_handler->reorderChild(m_containers[i]->handler(), orderInParentWidget);

        utils::logger()->trace("Zorder: {} Order in overlay: {}", m_containers[i]->zorder(), orderInParentWidget);
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
