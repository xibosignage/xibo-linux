#pragma once

#include "common/types/Color.hpp"
#include "common/types/Uri.hpp"

struct MainLayoutOptions
{
    int id;
    int width;
    int height;
    bool statEnabled;
    boost::optional<Uri> backgroundUri;
    Color backgroundColor;
};
