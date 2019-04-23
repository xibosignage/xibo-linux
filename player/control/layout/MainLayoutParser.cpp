#include "MainLayoutParser.hpp"

#include "MainLayoutResources.hpp"
#include "control/common/Validators.hpp"

const std::string DEFAULT_COLOR = "#000";

MainLayoutParser::MainLayoutParser(const xml_node& node) :
    m_layoutNode(node)
{
}

MainLayoutOptions MainLayoutParser::parse()
{
    int width = m_layoutNode.get<int>(ResourcesXlf::attr(ResourcesXlf::MainLayout::Width));
    int height = m_layoutNode.get<int>(ResourcesXlf::attr(ResourcesXlf::MainLayout::Height));
    auto uri = m_layoutNode.get_optional<std::string>(ResourcesXlf::attr(ResourcesXlf::MainLayout::BackgroundPath));
    auto color = m_layoutNode.get<std::string>(ResourcesXlf::attr(ResourcesXlf::MainLayout::BackgroundColor));
    color = color.empty() ? DEFAULT_COLOR : color;

    return MainLayoutOptions{width, height, Validators::validateUri(uri), Validators::validateColor(color)};
}
