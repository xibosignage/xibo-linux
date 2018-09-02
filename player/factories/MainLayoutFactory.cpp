#include "MainLayoutFactory.hpp"

#include "control/MainLayout.hpp"
#include "control/Background.hpp"

MainLayoutFactory::MainLayoutFactory(const xlf_node& layoutNode) :
    m_layoutNode(layoutNode)
{
}

std::unique_ptr<IMainLayout> MainLayoutFactory::create()
{
    auto attrs = m_layoutNode.get_child("<xmlattr>");

    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    std::string backgroundImage = attrs.get<std::string>("background", {});
    std::string backgroundColor = attrs.get<std::string>("bgcolor", {});

    return createLayout(width, height, backgroundImage, backgroundColor);
}

std::unique_ptr<IMainLayout> MainLayoutFactory::createLayout(int width, int height,
                                                             const std::string& backgroundImage,
                                                             const std::string& backgroundColor)
{
    auto layout = std::make_unique<MainLayout>(width, height);
    std::unique_ptr<IBackground> background;

    if(!backgroundColor.empty())
        layout->setBackground(Background::createOneColor(backgroundColor, width, height));

    if(!backgroundImage.empty())
        layout->setBackground(Background::createWithImage(backgroundImage, width, height));

    return layout;
}
