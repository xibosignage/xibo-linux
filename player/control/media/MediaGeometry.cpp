#include "MediaGeometry.hpp"
#include "common/Utils.hpp"

template <>
std::string Utils::toString(MediaGeometry::ScaleType val)
{
    switch (val)
    {
        case MediaGeometry::ScaleType::Scaled: return "Scaled";
        case MediaGeometry::ScaleType::Stretch: return "Stretch";
    }

    return "unknown";
}

template <>
std::string Utils::toString(MediaGeometry::Align val)
{
    switch (val)
    {
        case MediaGeometry::Align::Left: return "Left";
        case MediaGeometry::Align::Center: return "Center";
        case MediaGeometry::Align::Right: return "Right";
    }

    return "unknown";
}

template <>
std::string Utils::toString(MediaGeometry::Valign val)
{
    switch (val)
    {
        case MediaGeometry::Valign::Top: return "Top";
        case MediaGeometry::Valign::Middle: return "Middle";
        case MediaGeometry::Valign::Bottom: return "Bottom";
    }

    return "unknown";
}
