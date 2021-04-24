#pragma once

#include "stat/records/RecordType.hpp"

#include "common/dt/DateTime.hpp"

#include <optional>

namespace Stats
{
    struct RecordDto
    {
        int id;
        RecordType type;
        DateTime started;
        DateTime finished;
        int scheduleId;
        int layoutId;
        std::optional<int> mediaId;
        long duration;
        int count;
    };
}
