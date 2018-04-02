#pragma once

#include <string>
#include <glib.h>

const std::string LOGGER = "logger";
const int DEFAULT_X_POS = 0;
const int DEFAULT_Y_POS = 0;

// FIXME remove from constants.hpp
struct Point
{
    int left;
    int top;
};

// FIXME remove from constants.hpp
struct Size
{
    int width;
    int height;
};
