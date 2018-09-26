#include "MainLayoutFactory.hpp"

#include "control/MainLayout.hpp"
#include "control/Background.hpp"

#include "factories/BackgroundFactory.hpp"
#include "utils/utilities.hpp"

MainLayoutFactory::MainLayoutFactory(const xlf_node& layoutNode) :
    m_layoutNode(layoutNode)
{
}

#include "adaptors/GtkOverlayAdaptor.hpp"

std::unique_ptr<IMainLayout> MainLayoutFactory::create()
{
    int width = m_layoutNode.get<int>("<xmlattr>.width");
    int height = m_layoutNode.get<int>("<xmlattr>.height");
    auto imagePath = utils::resourcesDir() / m_layoutNode.get<std::string>("<xmlattr>.background", {});
    std::string color = m_layoutNode.get<std::string>("<xmlattr>.bgcolor", {});

    auto layout = std::make_unique<MainLayout>(std::make_unique<GtkOverlayAdaptor>());
    layout->setSize(width, height);
    layout->setBackground(BackgroundFactory().create(width, height, imagePath, color));
    return layout;
}
