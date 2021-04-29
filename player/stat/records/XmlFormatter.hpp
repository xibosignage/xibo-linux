#pragma once

#include "LayoutRecord.hpp"
#include "MediaRecord.hpp"
#include "RecordVisitor.hpp"
#include "Records.hpp"

#include "common/parsing/Parsing.hpp"
#include "common/PlayerRuntimeError.hpp"

namespace Stats
{
    class XmlFormatter : public RecordVisitor
    {
        DECLARE_EXCEPTION(Stats::XmlFormatter)        
    public:
        std::string format(const Records& records) const;

    protected:
        void visit(const LayoutRecord& record) override;
        void visit(const MediaRecord& record) override;

    private:
        void fillBase(const Record& record);

    private:
        XmlNode node_;
    };
}
