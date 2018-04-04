#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>

class RegionParser
{
public:
    RegionParser() = default;
    std::vector<int> media_ids() const;
    Params parse_region(int region_id);

private:
    int m_region_id;
};
