#include "Transition.hpp"
#include "common/Utils.hpp"

template <>
std::string Utils::toString(Transition::Heading val)
{
    switch (val)
    {
        case Transition::Heading::In: return "In";
        case Transition::Heading::Out: return "Out";
    }

    return "unknown";
}

template <>
std::string Utils::toString(Transition::Type val)
{
    switch (val)
    {
        case Transition::Type::Fade: return "Fade";
        case Transition::Type::Fly: return "Fly";
    }

    return "unknown";
}

template <>
std::string Utils::toString(Transition::Direction val)
{
    switch (val)
    {
        case Transition::Direction::N: return "N";
        case Transition::Direction::NE: return "NE";
        case Transition::Direction::E: return "E";
        case Transition::Direction::SE: return "SE";
        case Transition::Direction::S: return "S";
        case Transition::Direction::SW: return "SW";
        case Transition::Direction::W: return "W";
        case Transition::Direction::NW: return "NW";
    }

    return "unknown";
}
