#pragma once

#include "utils/constants.hpp"
#include <memory>

class Media;
class Region;

class MediaFactory
{
public:
    MediaFactory() = delete;
    static std::unique_ptr<Media> create(const Region& region, const Params& params);
};
