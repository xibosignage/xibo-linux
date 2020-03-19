#include "common/types/Uri.hpp"

Uri::Port::Port(unsigned short value) : StrongType{value} {}

Uri::Port Uri::Port::fromScheme(const Uri::Scheme& scheme)
{
    try
    {
        return DefaultPorts.at(scheme);
    }
    catch (std::exception&)
    {
        throw Error{"Uri::Port", "Default port for " + static_cast<std::string>(scheme) + " not found"};
    }
}

std::string Uri::Port::string() const
{
    return std::to_string(value_);
}

bool operator==(const Uri::Port& first, const Uri::Port& second)
{
    return static_cast<unsigned short>(first) == static_cast<unsigned short>(second);
}

bool operator!=(const Uri::Port& first, const Uri::Port& second)
{
    return !(first == second);
}
