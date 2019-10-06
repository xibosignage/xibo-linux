#include "ColorToHexConverter.hpp"

#include <regex>

const int ShortColorWithoutAlpha = 3;
const int ShortColorWithAlpha = 4;
const int LongColorWithoutAlpha = 6;
const int ColorBase = 16;
const std::string DefaultAlphaChannel = "FF";

uint32_t ColorToHexConverter::colorToHex(const std::string& color)
{
    if (!validColor(color))
        throw Error("ColorConverter", "HEX color should be 3, 4, 6, or 8 digits with # at the beginning");

    auto longColorWithAlpha = convertToLongColorWithAlpha(removeNumberSign(color));
    return static_cast<uint32_t>(std::stoul(longColorWithAlpha, nullptr, ColorBase));
}

bool ColorToHexConverter::validColor(const std::string& color) const
{
    std::regex hexColorRegex("^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3,4}|[A-Fa-f0-9]{8})$");

    if (!std::regex_match(color, hexColorRegex)) return false;

    return true;
}

std::string ColorToHexConverter::removeNumberSign(const std::string& color)
{
    size_t positionAfterNumberSign = color.find('#') + 1;
    return color.substr(positionAfterNumberSign);
}

std::string ColorToHexConverter::convertToLongColorWithAlpha(const std::string& color)
{
    std::string resultColor = color;

    if (isShortColor(resultColor)) resultColor = convertShortToLongColor(resultColor);

    if (isColorWithoutAlpha(resultColor)) resultColor = appendDefaultAlphaChannel(resultColor);

    return resultColor;
}

bool ColorToHexConverter::isShortColor(const std::string& color) const
{
    return color.size() == ShortColorWithAlpha || color.size() == ShortColorWithoutAlpha;
}

std::string ColorToHexConverter::convertShortToLongColor(const std::string& shortColor)
{
    std::string longColor;
    for (auto&& digit : shortColor)
    {
        longColor += doubleDigit(digit);
    }
    return longColor;
}

std::string ColorToHexConverter::doubleDigit(char digit)
{
    return std::string(2, digit);
}

bool ColorToHexConverter::isColorWithoutAlpha(const std::string& color) const
{
    return color.size() == LongColorWithoutAlpha;
}

std::string ColorToHexConverter::appendDefaultAlphaChannel(const std::string& longColorWithoutAlpha)
{
    return longColorWithoutAlpha + DefaultAlphaChannel;
}
