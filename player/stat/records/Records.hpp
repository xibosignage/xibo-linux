#pragma once

#include "Container.hpp"
#include "MediaRecord.hpp"
#include "Record.hpp"

namespace Stats
{
    using Records = Container<std::unique_ptr<Record>>;
    using MediaRecords = Container<std::unique_ptr<MediaRecord>>;
}
