#include "XmlFormatter.hpp"

using namespace Stats;

std::string XmlFormatter::format(const PlayingRecords& records) const
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

void XmlFormatter::visit(const LayoutPlayingRecord& record)
{
    node_.put(Parsing::xmlAttr("type"), Stats::recordTypeToString(PlayingRecordType::Layout));
    node_.put(Parsing::xmlAttr("fromdt"), record.started().string("%Y-%m-%d %H:%M:%S"));
    node_.put(Parsing::xmlAttr("todt"), record.finished().string("%Y-%m-%d %H:%M:%S"));
    node_.put(Parsing::xmlAttr("scheduleid"), std::to_string(record.scheduleId()));
    node_.put(Parsing::xmlAttr("layoutid"), std::to_string(record.id()));
    node_.put(Parsing::xmlAttr("mediaid"), "");
    node_.put(Parsing::xmlAttr("duration"), std::to_string(record.duration()));
    node_.put(Parsing::xmlAttr("count"), std::to_string(record.count()));
}

void XmlFormatter::visit(const MediaPlayingRecord& record)
{
    node_.put(Parsing::xmlAttr("type"), Stats::recordTypeToString(PlayingRecordType::Media));
    node_.put(Parsing::xmlAttr("fromdt"), record.started().string("%Y-%m-%d %H:%M:%S"));
    node_.put(Parsing::xmlAttr("todt"), record.finished().string("%Y-%m-%d %H:%M:%S"));
    node_.put(Parsing::xmlAttr("scheduleid"), std::to_string(record.scheduleId()));
    node_.put(Parsing::xmlAttr("layoutid"), std::to_string(record.parentId()));
    node_.put(Parsing::xmlAttr("mediaid"), record.id());
    node_.put(Parsing::xmlAttr("duration"), std::to_string(record.duration()));
    node_.put(Parsing::xmlAttr("count"), std::to_string(record.count()));
}
