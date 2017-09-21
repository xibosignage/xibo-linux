#include "Region.hpp"

void Region::InitOptions(std::map<wxString, wxString> options)
{
    this->options.loop = std::stoi(options["loop"].ToStdString());
    // init transition
}
