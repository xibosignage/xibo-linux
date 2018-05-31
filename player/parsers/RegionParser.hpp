#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>

class Region;

class RegionParser
{
public:
    RegionParser(const xlf_node& region_node);
    std::unique_ptr<Region> parse();

private:
    struct ParsedRegion
    {
        int id;
        int width;
        int height;
        int top;
        int left;
        int zindex;
        bool loop;
    };

    ParsedRegion parse_params();

private:
    xlf_node m_region_node;

};
