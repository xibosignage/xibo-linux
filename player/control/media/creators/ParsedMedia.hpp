#pragma once

#include "../MediaOptions.hpp"

#include <memory>
#include <map>

using ExtraOptions = std::map<std::string, std::string>;

struct ParsedMedia
{
    MediaOptions baseOptions;
    ExtraOptions extraOptions;
    std::unique_ptr<ParsedMedia> attachedMedia;
};
