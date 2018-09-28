#pragma once

#include <memory>
#include <vector>

class IMainLayout;
class IBackground;
class IMediaContainer;

struct MediaContainerStruct
{
    std::unique_ptr<IMediaContainer> container;
    int x;
    int y;
};

class MainLayoutBuilder
{
public:
    template<typename MainLayoutFactory>
    std::unique_ptr<IMainLayout> build()
    {
        auto layout = MainLayoutFactory().create();

        prepareLayout(*layout);

        return layout;
    }

    MainLayoutBuilder& setWidth(int width);
    MainLayoutBuilder& setHeight(int height);
    MainLayoutBuilder& setBackground(std::unique_ptr<IBackground>&& background);
    MainLayoutBuilder& setMediaContainers(std::vector<MediaContainerStruct>&& mediaContainers);

private:
    void prepareLayout(IMainLayout& layout);

private:
    int m_width;
    int m_height;
    std::unique_ptr<IBackground> m_background;
    std::vector<MediaContainerStruct> m_mediaContainers;

};
