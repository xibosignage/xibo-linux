#include "MainLayoutParser.hpp"
#include "MainLayoutResources.hpp"

#include "control/region/RegionParser.hpp"
#include "control/common/Validators.hpp"

const std::string DEFAULT_COLOR = "#000";


ParsedLayout MainLayoutParser::parse(const xml_node& node)
{
    ParsedLayout layout;

    layout.options.width = node.get<int>(ResourcesXlf::attr(ResourcesXlf::MainLayout::Width));
    layout.options.height = node.get<int>(ResourcesXlf::attr(ResourcesXlf::MainLayout::Height));

    layout.options.backgroundUri = getUri(node);
    layout.options.backgroundColor = getColor(node);
    layout.regions = parseRegions(node);

    return layout;
}

Uri MainLayoutParser::getUri(const xml_node& node)
{
    auto uri = node.get_optional<std::string>(ResourcesXlf::attr(ResourcesXlf::MainLayout::BackgroundPath));

    return Validators::validateUri(uri);
}

uint32_t MainLayoutParser::getColor(const xml_node& node)
{
    auto color = node.get<std::string>(ResourcesXlf::attr(ResourcesXlf::MainLayout::BackgroundColor));

    color = color.empty() ? DEFAULT_COLOR : color;

    return Validators::validateColor(color);
}

std::vector<ParsedRegion> MainLayoutParser::parseRegions(const xml_node& node)
{
    std::vector<ParsedRegion> regions;

    for(auto [nodeName, regionNode] : node)
    {
        if(nodeName == ResourcesXlf::RegionNode)
        {
            RegionParser parser;
            regions.emplace_back(parser.parse(regionNode));
        }
    }

    return regions;
}
