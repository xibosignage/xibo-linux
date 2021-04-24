#include "RecordType.hpp"

std::optional<Stats::RecordType> Stats::recordTypeFromSting(const std::string& t)
{
    if (t == "layout")
    {
        return RecordType::Layout;
    }
    else if (t == "media")
    {
        return RecordType::Media;
    }
    return {};
}

std::string Stats::recordTypeToString(Stats::RecordType t)
{
    switch (t)
    {
        case RecordType::Layout: return "layout";
        case RecordType::Media: return "media";
    }
    return {};
}
