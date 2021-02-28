#pragma once

#include "PlayingRecord.hpp"

namespace Stats
{
    class RecordVisitor
    {
    public:
        virtual ~RecordVisitor() = default;
        virtual void visit(const LayoutPlayingRecord& record) = 0;
        virtual void visit(const MediaPlayingRecord& record) = 0;
    };
}
