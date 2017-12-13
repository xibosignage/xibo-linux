#pragma once

#include "MediaParser.hpp"
#include "Image.hpp"

struct ParsedImage : ParsedMedia
{
    ParsedImage(int _id, int _duration, const std::string& _uri, Image::ScaleType _scale_type, Image::Align _align, Image::Valign _valign) :
        ParsedMedia{_id, _duration, _uri}, scale_type(_scale_type), align(_align), valign(_valign) { }

    Image::ScaleType scale_type;
    Image::Align align;
    Image::Valign valign;
};


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
