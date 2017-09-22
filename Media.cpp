#include "Media.hpp"

#include "utilities.hpp"

void Media::InitOptions(std::map<wxString, wxString> options)
{    
    this->options.uri = utilities::GetValue<wxString>(options["uri"]).value();
}
