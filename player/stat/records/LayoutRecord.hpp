#pragma once

#include "Record.hpp"

namespace Stats
{
    class LayoutRecord : public Record
    {
        using Record::Record;

    public:
        static std::unique_ptr<LayoutRecord> create(int scheduleId, int id, const PlayingTime& interval, int count = 1);
        void apply(RecordVisitor& visitor) override;
    };
}