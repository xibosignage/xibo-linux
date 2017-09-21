#include "Image.hpp"

void Image::InitOptions(std::map<wxString, wxString> options)
{
    Media::InitOptions(options);
    this->options.scaleType = utilities::GetValue<int>(options["scaleType"]).value_or(0);
    this->options.align = utilities::GetValue<int>(options["align"]).value_or(0);
    this->options.valign = utilities::GetValue<int>(options["valign"]).value_or(0);
}
