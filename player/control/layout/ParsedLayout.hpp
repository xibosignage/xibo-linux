#pragma once

#include "control/region/ParsedRegion.hpp"

struct LayoutOptions
{
    int width;
    int height;
    Uri backgroundUri;
    uint32_t backgroundColor;
};

struct ParsedLayout
{
    LayoutOptions options;
    std::vector<ParsedRegion> regions;
};
