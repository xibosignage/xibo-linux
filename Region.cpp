#include "Region.hpp"
#include "utilities.hpp"

void Region::InitOptions(std::map<std::string, std::string> options)
{
    this->options.loop = utilities::GetValue<bool>(options["loop"]).value_or(false);
}
