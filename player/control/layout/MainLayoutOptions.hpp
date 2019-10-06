#pragma once

#include "common/types/Color.hpp"
#include "common/types/Uri.hpp"

struct MainLayoutOptions
{
    int width;
    int height;
    boost::optional<Uri> backgroundUri;
    Color backgroundColor;
};
