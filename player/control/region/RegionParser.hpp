#pragma once

#include "constants.hpp"
#include "control/media/MediaOptions.hpp"
#include "control/region/Region.hpp"
#include "control/region/RegionOptions.hpp"

struct RegionPosition
{
    int left;
    int top;
    int zorder;
};

class RegionParser
{
public:
    struct Error : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    std::unique_ptr<Xibo::Region> regionFrom(const PtreeNode& node);
    RegionPosition positionFrom(const PtreeNode& node);

private:
    RegionOptions optionsFrom(const PtreeNode& node);
    void addMedia(Xibo::Region& region, const PtreeNode& node);
    MediaOptions::Type mediaTypeFrom(const PtreeNode& node);
};
