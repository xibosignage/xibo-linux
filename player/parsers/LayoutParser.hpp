#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>

class LayoutParser
{
public:
    LayoutParser() = default;
    std::vector<int> regions_ids() const;
    Params parse_layout();
};
