#pragma once

#include <vector>
#include <cstdint>

struct Size
{
    int width;
    int height;
};

struct Point
{
    int x;
    int y;
};

using ImageData = std::vector<uint8_t>;
