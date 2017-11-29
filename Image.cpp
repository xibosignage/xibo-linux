#include "Image.hpp"

void Image::InitOptions(std::map<std::string, std::string> options)
{
    Media::InitOptions(options);
    this->options.scaleType = utilities::GetValue<ScaleType>(options["scaleType"]).value_or(ScaleType::Center);
    this->options.align = utilities::GetValue<Align>(options["align"]).value_or(Align::Center);
    this->options.valign = utilities::GetValue<Valign>(options["valign"]).value_or(Valign::Middle);
}
