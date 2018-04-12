#pragma once

#include "constants.hpp"
#include <memory>

class Region;
class ParsedRegion;

class RegionBuilder
{
public:
    RegionBuilder() = delete;
    static std::unique_ptr<Region> create(const ParsedRegion& object);

private:
    static int available_index;
    static std::unique_ptr<Region> create_from_params(const ParsedRegion& object);

};
