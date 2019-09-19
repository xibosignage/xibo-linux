#include "MediaPlayerOptions.hpp"
#include "common/Utils.hpp"

template<>
std::string Utils::toString(MediaPlayerOptions::Mute val)
{
    switch (val)
    {
        case MediaPlayerOptions::Mute::Disable: return "Disable";
        case MediaPlayerOptions::Mute::Enable: return "Enable";
    }

    return "unknown";
}

template<>
std::string Utils::toString(MediaPlayerOptions::Loop val)
{
    switch (val)
    {
        case MediaPlayerOptions::Loop::Disable: return "Disable";
        case MediaPlayerOptions::Loop::Enable: return "Enable";
    }

    return "unknown";
}
