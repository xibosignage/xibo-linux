#pragma once

#include <optional>

namespace Stats
{
    enum class RecordType
    {
        Layout,
        Media
    };

    std::string recordTypeToString(RecordType t);
    std::optional<RecordType> recordTypeFromSting(const std::string& t);
}
