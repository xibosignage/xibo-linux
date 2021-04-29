#include "XmlFormatter.hpp"

using namespace Stats;
using namespace std::string_literals;

std::string XmlFormatter::format(const Records& records) const
{
    try
    {
        XmlNode root;
        auto&& stats = root.put_child("stats", XmlNode{});

        for (auto&& record : records)
        {
            XmlFormatter formatter;
            record->apply(formatter);

            stats.add_child("stat", formatter.node_);
        }

        return Parsing::xmlTreeToString(root);
    }
    catch (const std::exception& e)
    {
        throw Error{"Failed to format stat: "s + e.what()};
    }
}

void XmlFormatter::fillBase(const Record& record)
{
    node_.put(Parsing::xmlAttr("scheduleid"), std::to_string(record.scheduleId()));
    node_.put(Parsing::xmlAttr("fromdt"), record.started().string("%Y-%m-%d %H:%M:%S"));
    node_.put(Parsing::xmlAttr("todt"), record.finished().string("%Y-%m-%d %H:%M:%S"));
    node_.put(Parsing::xmlAttr("duration"), std::to_string(record.duration()));
    node_.put(Parsing::xmlAttr("count"), std::to_string(record.count()));
}

void XmlFormatter::visit(const LayoutRecord& record)
{
    fillBase(record);

    node_.put(Parsing::xmlAttr("type"), Stats::recordTypeToString(RecordType::Layout));
    node_.put(Parsing::xmlAttr("layoutid"), std::to_string(record.id()));
    node_.put(Parsing::xmlAttr("mediaid"), "");
}

void XmlFormatter::visit(const MediaRecord& record)
{
    fillBase(record);

    node_.put(Parsing::xmlAttr("type"), Stats::recordTypeToString(RecordType::Media));
    node_.put(Parsing::xmlAttr("layoutid"), std::to_string(record.parentId()));
    node_.put(Parsing::xmlAttr("mediaid"), record.id());
}
