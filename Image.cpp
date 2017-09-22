#include "Image.hpp"

void Image::InitOptions(std::map<wxString, wxString> options)
{
    Media::InitOptions(options);
    this->options.scaleType = utilities::GetValue<ScaleType>(options["scaleType"]).value_or(ScaleType::Center);
    this->options.align = utilities::GetValue<Align>(options["align"]).value_or(Align::Center);
    this->options.valign = utilities::GetValue<Valign>(options["valign"]).value_or(Valign::Middle);
}
