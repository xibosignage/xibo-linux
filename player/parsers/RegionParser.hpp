#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>
#include <boost/property_tree/ptree.hpp>

class Region;

class RegionParser
{
public:
    RegionParser(const xlf_node& regionNode);
    std::unique_ptr<Region> parse();
    std::vector<xlf_node>::const_iterator begin() const;
    std::vector<xlf_node>::const_iterator end() const;

private:
    xlf_node m_regionNode;
    std::vector<xlf_node> m_mediaNodes;

};
