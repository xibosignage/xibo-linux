#include "LayoutParser.hpp"
#include "control/MainLayout.hpp"
#include "control/Region.hpp"
#include "parsers/RegionParser.hpp"

LayoutParser::LayoutParser(const xlf_node& layout_node) :
    m_layout_node(layout_node)
{
    for(auto [layout_node_name, region_node] : m_layout_node)
    {
        if(layout_node_name == "region")
        {
            m_region_nodes.push_back(region_node);
        }
    }
}

#include "control/Background.hpp"
#include "utils/utilities.hpp"

std::unique_ptr<MainLayout> LayoutParser::parse()
{
    //spdlog::get(LOGGER)->trace("Parsing layout...");
    auto attrs = m_layout_node.get_child("<xmlattr>");

    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    std::string bgimage = attrs.get<std::string>("background", {});
    std::string bgcolor = attrs.get<std::string>("bgcolor", {});

    auto layout =  std::make_unique<MainLayout>(width, height);

    if(!bgcolor.empty())
        layout->set_background(Background::create_one_color(bgcolor, width, height));
    if(!bgimage.empty())
        layout->set_background(Background::create_with_image(bgimage, width, height));

    return layout;
}

std::vector<xlf_node>::const_iterator LayoutParser::begin() const
{
    return std::cbegin(m_region_nodes);
}

std::vector<xlf_node>::const_iterator LayoutParser::end() const
{
    return std::cend(m_region_nodes);
}
