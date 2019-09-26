#pragma once

#include <string>

class ColorToHexConverter
{
public:
    uint32_t colorToHex(const std::string& color);

private:
    bool isValidColor(const std::string& hexColor) const;
    std::string removeNumberSign(const std::string& color);
    std::string convertToLongColorWithAlpha(const std::string& hexColor);
    bool isShortColor(const std::string& hexColor) const;
    std::string convertShortToLongColor(const std::string& hexColor);
    std::string doubleDigit(char digit);
    bool isColorWithoutAlpha(const std::string& hexColor) const;
    std::string appendDefaultAlphaChannel(const std::string& hexColor);
};
