#include "MainLayoutParser.hpp"

#include "XlfResources.hpp"

const std::string DEFAULT_COLOR = "#000";

MainLayoutParser::MainLayoutParser(const xml_node& node) :
    m_layoutNode(node)
{
}

LayoutOptions MainLayoutParser::parse()
{
    int width = m_layoutNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Layout::Width));
    int height = m_layoutNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Layout::Height));
    auto uri = m_layoutNode.get_optional<std::string>(ResourcesXlf::attr(ResourcesXlf::Background::Path));
    auto color = m_layoutNode.get<std::string>(ResourcesXlf::attr(ResourcesXlf::Background::Color), DEFAULT_COLOR);

    return LayoutOptions{width, height, uri, color};
}
