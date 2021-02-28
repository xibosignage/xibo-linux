#pragma once

#include <optional>

namespace Stats
{
    enum class PlayingRecordType
    {
        Layout,
        Media
    };

    std::string recordTypeToString(PlayingRecordType t);
    std::optional<PlayingRecordType> recordTypeFromSting(const std::string& t);
}
