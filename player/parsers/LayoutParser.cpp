#include "LayoutParser.hpp"
#include "control/MainLayout.hpp"
#include "control/Region.hpp"
#include "parsers/RegionParser.hpp"

LayoutParser::LayoutParser(const xlf_node& layoutNode) :
    m_layoutNode(layoutNode)
{
    for(auto [layoutNodeName, regionNode] : m_layoutNode)
    {
        if(layoutNodeName == "region")
        {
            m_regionNodes.push_back(regionNode);
        }
    }
}

#include "control/Background.hpp"
#include "utils/utilities.hpp"

std::unique_ptr<MainLayout> LayoutParser::parse()
{
    //spdlog::get(LOGGER)->trace("Parsing layout...");
    auto attrs = m_layoutNode.get_child("<xmlattr>");

    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    std::string bgimage = attrs.get<std::string>("background", {});
    std::string bgcolor = attrs.get<std::string>("bgcolor", {});

    auto layout =  std::make_unique<MainLayout>(width, height);

    if(!bgcolor.empty())
        layout->setBackground(Background::createOneColor(bgcolor, width, height));
    if(!bgimage.empty())
        layout->setBackground(Background::createWithImage(bgimage, width, height));

    return layout;
}

std::vector<xlf_node>::const_iterator LayoutParser::begin() const
{
    return std::cbegin(m_regionNodes);
}

std::vector<xlf_node>::const_iterator LayoutParser::end() const
{
    return std::cend(m_regionNodes);
}
