#pragma once

#include "constants.hpp"
#include "parsers/ParserHelpers.hpp"
#include <spdlog/spdlog.h>

class RegionParser
{
private:
    RegionParser(const xlf_node& region_node);
    ParsedRegion parse_region();
    ParsedRegion parse_region_params();

    friend class LayoutParser;

private:
    const xlf_node& m_region_node;
};
