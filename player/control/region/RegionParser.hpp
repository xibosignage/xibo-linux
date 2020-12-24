#pragma once

#include "common/parsing/Parsing.hpp"
#include "common/PlayerRuntimeError.hpp"
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
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    RegionParser(bool globalStatEnabled);

    std::unique_ptr<Xibo::Region> regionFrom(const XmlNode& node);
    RegionPosition positionFrom(const XmlNode& node);

private:
    RegionOptions optionsFrom(const XmlNode& node);
    void addMedia(Xibo::Region& region, const XmlNode& node);
    MediaOptions::Type mediaTypeFrom(const XmlNode& node);

private:
    bool globalStatEnabled_;
};
