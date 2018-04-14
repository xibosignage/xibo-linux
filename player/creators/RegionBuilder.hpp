#pragma once

#include "constants.hpp"
#include <memory>

class Region;
class ParsedRegion;

class RegionBuilder
{
public:
    RegionBuilder(const ParsedRegion& params);
    std::unique_ptr<Region> build();
    RegionBuilder& set_width_scale_factor(double width_scale_factor);
    RegionBuilder& set_height_scale_factor(double height_scale_factor);

private:
    std::unique_ptr<Region> create_from_params();

private:
    static int available_index;
    const ParsedRegion& m_params;
    double m_width_scale_factor;
    double m_height_scale_factor;

};
