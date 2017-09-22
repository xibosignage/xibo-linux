#include "Region.hpp"
#include "utilities.hpp"

void Region::InitOptions(std::map<wxString, wxString> options)
{
    this->options.loop = utilities::GetValue<bool>(options["loop"]).value_or(false);
}
