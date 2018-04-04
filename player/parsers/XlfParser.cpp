#include "XlfParser.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include <spdlog/spdlog.h>

void XlfParser::parse_xlf(const std::string& path)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(path, tree);

    auto layout_node = tree.get_child("layout");
    m_layout.add_child("<xmlattr>", layout_node.get_child("<xmlattr>"));

    for(auto [layout_node_name, region_node] : layout_node)
    {
        if(layout_node_name == "region")
        {
            int region_id = region_node.get_child("<xmlattr>").get<int>("id");
            m_regions_ids.push_back(region_id);
            auto& region = m_layout.add_child(std::to_string(region_id), region_node);
            for(auto [region_node_name, media_node] : region)
            {
                if(region_node_name == "media")
                {
                    int media_id = region_node.get_child("<xmlattr>").get<int>("id");
                    m_media_ids[region_id].push_back(media_id);
                    region.add_child(std::to_string(media_id), media_node);                }
            }
        }
    }
}

std::vector<int> XlfParser::regions_ids() const
{
    return m_regions_ids;
}

std::vector<int> XlfParser::media_ids(int region_id) const
{
    return m_media_ids.at(region_id);
}

xlf_node XlfParser::layout_node() const
{
    return m_layout;
}

xlf_node XlfParser::region_node(int region_id) const
{
    return m_layout.get_child(std::to_string(region_id));
}

xlf_node XlfParser::media_node(int region_id, int media_id) const
{
    return m_layout.get_child(std::to_string(region_id)).get_child(std::to_string(media_id));
}
