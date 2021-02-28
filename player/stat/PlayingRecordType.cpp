#include "PlayingRecordType.hpp"

std::optional<Stats::PlayingRecordType> Stats::recordTypeFromSting(const std::string& t)
{
    if (t == "layout")
    {
        return PlayingRecordType::Layout;
    }
    else if (t == "media")
    {
        return PlayingRecordType::Media;
    }
    return {};
}

std::string Stats::recordTypeToString(Stats::PlayingRecordType t)
{
    switch (t)
    {
        case PlayingRecordType::Layout: return "layout";
        case PlayingRecordType::Media: return "media";
    }
    return {};
}
