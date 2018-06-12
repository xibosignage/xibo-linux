#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>
#include <gtest/gtest_prod.h>
#include <boost/property_tree/ptree.hpp>

class Region;

class RegionParser
{
public:
    RegionParser(const xlf_node& region_node);
    std::unique_ptr<Region> parse();
    std::vector<xlf_node>::const_iterator begin() const;
    std::vector<xlf_node>::const_iterator end() const;

private:
    xlf_node m_region_node;
    std::vector<xlf_node> m_media_nodes;

};
