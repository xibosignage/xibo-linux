#pragma once

#include "RecordDto.hpp"
#include "stat/records/RecordVisitor.hpp"

namespace Stats
{
    class DtoConverter : public RecordVisitor
    {
    public:
        RecordDto dto() const;

    protected:
        void visit(const LayoutRecord& record) override;
        void visit(const MediaRecord& record) override;

    private:
        void fillGenericData(const Record& record);

    private:
        RecordDto dto_;
    };
}
