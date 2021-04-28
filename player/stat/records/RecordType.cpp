#include "RecordType.hpp"

const std::string LayoutType{"layout"};
const std::string MediaType{"media"};

std::optional<Stats::RecordType> Stats::recordTypeFromSting(const std::string& t)
{
    if (t == LayoutType)
    {
        return RecordType::Layout;
    }
    else if (t == MediaType)
    {
        return RecordType::Media;
    }
    return {};
}

std::string Stats::recordTypeToString(Stats::RecordType t)
{
    switch (t)
    {
        case RecordType::Layout: return LayoutType;
        case RecordType::Media: return MediaType;
    }
    return {};
}
