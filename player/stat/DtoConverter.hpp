#pragma once

#include "IRecordVisitor.hpp"
#include "PlayingRecordDto.hpp"

namespace Stats
{
    class DtoConverter : public RecordVisitor
    {
    public:
        PlayingRecordDto dto() const;

    protected:
        void visit(const LayoutPlayingRecord& record) override;
        void visit(const MediaPlayingRecord& record) override;

    private:
        void fillGenericData(const PlayingRecord& record);

    private:
        PlayingRecordDto dto_;
    };
}
