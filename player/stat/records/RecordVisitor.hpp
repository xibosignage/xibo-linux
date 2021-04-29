#pragma once

#include "LayoutRecord.hpp"
#include "MediaRecord.hpp"

namespace Stats
{
    class RecordVisitor
    {
    public:
        virtual ~RecordVisitor() = default;
        virtual void visit(const LayoutRecord& record) = 0;
        virtual void visit(const MediaRecord& record) = 0;
    };
}
