#pragma once

#include <vector>

#include "control/media/creators/ParsedMedia.hpp"

struct RegionOptions
{
    enum class Loop
    {
        Disable,
        Enable
    };

    int id;
    int width;
    int height;
    int left;
    int top;
    int zindex;
    Loop loop;
};

struct ParsedRegion
{
    RegionOptions options;
    std::vector<ParsedMedia> media;
};
