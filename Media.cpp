#include "Media.hpp"

#include "utilities.hpp"

void Media::InitOptions(std::map<std::string, std::string> options)
{
    this->options.uri = utilities::GetValue<std::string>(options["uri"]).value();
}
