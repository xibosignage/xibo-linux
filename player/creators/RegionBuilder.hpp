#pragma once

#include "utils/constants.hpp"
#include <memory>

class Region;

class RegionBuilder
{
public:
    RegionBuilder() = delete;
    static std::unique_ptr<Region> create(const Params& params);

};
