#pragma once

#include "PlayingRecord.hpp"
#include "Records.hpp"

namespace Stats
{
    using PlayingRecords = Records<std::unique_ptr<PlayingRecord>>;
    using MediaPlayingRecords = Records<std::unique_ptr<MediaPlayingRecord>>;
}
