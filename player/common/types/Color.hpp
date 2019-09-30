#pragma once

#include "common/types/ColorToHexConverter.hpp"
#include "common/types/StrongType.hpp"

struct Color
{
    // TODO: remove default color
    Color() = default;

    static Color fromString(const std::string& color)
    {
        ColorToHexConverter converter;
        return Color{converter.colorToHex(color), color};
    }

    const std::string& string() const
    {
        return colorStr_;
    }

    uint32_t hex() const
    {
        return colorHex_;
    }

private:
    Color(uint32_t colorHex, const std::string& colorStr) : colorHex_(colorHex), colorStr_(colorStr) {}

private:
    uint32_t colorHex_;
    std::string colorStr_;
};
