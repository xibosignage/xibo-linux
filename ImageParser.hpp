#pragma once

#include "MediaParser.hpp"
#include "Image.hpp"

using ParsedImage = std::tuple<uint, uint, bool, std::string, Image::ScaleType, Image::Align, Image::Valign>;

class ImageParser : public MediaParser<ParsedImage>
{
public:
    using MediaParser<ParsedImage>::MediaParser;
    ParsedImage parse();

private:
    Image::ScaleType from_scale_type(const std::string& option_name);
    Image::Align from_align(const std::string& option_name);
    Image::Valign from_valign(const std::string& option_name);

};
