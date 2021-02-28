#pragma once

#include "IRecordVisitor.hpp"
#include "PlayingRecords.hpp"

#include "common/parsing/Parsing.hpp"

namespace Stats
{
    class XmlFormatter : public RecordVisitor
    {
    public:
        std::string format(const PlayingRecords& records) const;

    protected:
        void visit(const LayoutPlayingRecord& record) override;
        void visit(const MediaPlayingRecord& record) override;

    private:
        XmlNode node_;
    };
}
