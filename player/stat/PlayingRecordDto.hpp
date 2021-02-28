#pragma once

#include "PlayingRecordType.hpp"

#include "common/dt/DateTime.hpp"

#include <optional>

namespace Stats
{
    struct PlayingRecordDto
    {
        int id;
        PlayingRecordType type;
        DateTime started;
        DateTime finished;
        int scheduleId;
        int layoutId;
        std::optional<int> mediaId;
        long duration;
        int count;
    };
}
