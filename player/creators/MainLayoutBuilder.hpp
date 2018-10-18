#pragma once

#include <memory>
#include <vector>

#include "control/IMainLayout.hpp"
#include "control/IBackground.hpp"
#include "control/IMediaContainer.hpp"

#include "adaptors/IOverlayAdaptor.hpp"

struct MediaContainerWithPos
{
    std::unique_ptr<IMediaContainer> container;
    int x;
    int y;
};

class MainLayoutBuilder
{
public:
    std::unique_ptr<IMainLayout> build();

    MainLayoutBuilder& adaptor(std::unique_ptr<IOverlayAdaptor>&& adaptor);
    MainLayoutBuilder& width(int width);
    MainLayoutBuilder& height(int height);
    MainLayoutBuilder& background(std::unique_ptr<IBackground>&& background);
    MainLayoutBuilder& mediaContainers(std::vector<MediaContainerWithPos>&& mediaContainers);

protected:
    virtual std::unique_ptr<IMainLayout> createLayout();

private:
    void prepareLayout(IMainLayout& layout);
    void checkWidth(int width);
    void checkHeight(int height);

private:
    std::unique_ptr<IOverlayAdaptor> m_adaptor;
    int m_width;
    int m_height;
    std::unique_ptr<IBackground> m_background;
    std::vector<MediaContainerWithPos> m_mediaContainers;

};
