#pragma once

#include <string>
#include <map>
#include "constants.hpp"

class XlfParser
{
public:
    XlfParser() = default;
    void parse_xlf(const std::string& path);

    friend class LayoutParser;
    friend class RegionParser;
    friend class MediaParser;

private:
    std::vector<int> regions_ids() const;
    std::vector<int> media_ids(int region_id) const;

    xlf_node layout_node() const;
    xlf_node region_node(int region_id) const;
    xlf_node media_node(int region_id, int media_id) const;

private:
    boost::property_tree::ptree m_layout;
    std::vector<int> m_regions_ids;
    std::map<int, std::vector<int>> m_media_ids;
};
