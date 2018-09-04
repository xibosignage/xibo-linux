#include "MainLayoutFactory.hpp"

#include "control/MainLayout.hpp"
#include "control/Background.hpp"

#include "factories/BackgroundFactory.hpp"
#include "utils/utilities.hpp"

MainLayoutFactory::MainLayoutFactory(const xlf_node& layoutNode) :
    m_layoutNode(layoutNode)
{
}

std::unique_ptr<IMainLayout> MainLayoutFactory::create()
{
    auto attrs = m_layoutNode.get_child("<xmlattr>");

    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    auto imagePath = utils::resourcesDir() / attrs.get<std::string>("background", {});
    std::string color = attrs.get<std::string>("bgcolor", {});

    return createLayout(width, height, imagePath, color);
}

std::unique_ptr<IMainLayout> MainLayoutFactory::createLayout(int width, int height,
                                                             const std::filesystem::path& imagePath,
                                                             const std::string& color)
{
    auto layout = std::make_unique<MainLayout>(width, height);
    layout->setBackground(BackgroundFactory().create(width, height, imagePath, color));
    return layout;
}
