#include "Video.hpp"

#include "utilities.hpp"

void Video::InitOptions(std::map<wxString, wxString> options)
{
    Media::InitOptions(options);
    this->options.mute = utilities::GetValue<bool>(options["mute"]).value_or(false);
    this->options.loop = utilities::GetValue<bool>(options["loop"]).value_or(false);
}
