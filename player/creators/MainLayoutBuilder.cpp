#include "MainLayoutBuilder.hpp"

#include "control/IMainLayout.hpp"
#include "control/IBackground.hpp"
#include "control/IMediaContainer.hpp"

MainLayoutBuilder& MainLayoutBuilder::setWidth(int width)
{
    m_width = width;
    return *this;
}

MainLayoutBuilder& MainLayoutBuilder::setHeight(int height)
{
    m_height = height;
    return *this;
}

MainLayoutBuilder& MainLayoutBuilder::setBackground(std::unique_ptr<IBackground>&& background)
{
    m_background = std::move(background);
    return *this;
}

MainLayoutBuilder& MainLayoutBuilder::setMediaContainers(std::vector<MediaContainerStruct>&& mediaContainers)
{
    m_mediaContainers = std::move(mediaContainers);
    return *this;
}

void MainLayoutBuilder::prepareLayout(IMainLayout& layout)
{
    layout.setSize(m_width, m_height);
    layout.setBackground(std::move(m_background));

    for(auto&& ct : m_mediaContainers)
    {
        layout.addMediaContainer(std::move(ct.container), ct.x, ct.y);
    }
}
