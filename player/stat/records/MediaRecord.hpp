#pragma once

#include "Record.hpp"

namespace Stats
{
    class MediaRecord : public Record
    {
        using Record::Record;

    public:
        static std::unique_ptr<MediaRecord> create(int scheduleId,
                                                   int parentId,
                                                   int id,
                                                   const PlayingTime& interval,
                                                   int count = 1);

        void apply(RecordVisitor& visitor) override;
    };
}
