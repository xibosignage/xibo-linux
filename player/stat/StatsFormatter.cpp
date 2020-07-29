#include "StatsFormatter.hpp"

std::string StatsFormatter::toXml(const StatsRecorder::Records& records) const
{
    XmlNode root;
    auto&& stats = root.put_child("stats", XmlNode{});

    for (auto&& record : records)
    {
        stats.add_child("stat", createRecordNode(record));
    }

    return Parsing::xmlTreeToString(root);
}

XmlNode StatsFormatter::createRecordNode(const StatsRecorder::Record& record) const
{
    XmlNode recordNode;

    recordNode.put(Parsing::xmlAttr("type"), formatType(record.type));
    recordNode.put(Parsing::xmlAttr("fromdt"), record.started.string("%Y-%m-%d %H:%M:%S"));
    recordNode.put(Parsing::xmlAttr("todt"), record.finished.string("%Y-%m-%d %H:%M:%S"));
    recordNode.put(Parsing::xmlAttr("scheduleid"), std::to_string(record.scheduleId));
    recordNode.put(Parsing::xmlAttr("layoutid"), std::to_string(record.layoutId));
    recordNode.put(Parsing::xmlAttr("mediaid"), formatMediaId(record.mediaId));
    recordNode.put(Parsing::xmlAttr("duration"), std::to_string(record.duration));
    recordNode.put(Parsing::xmlAttr("count"), std::to_string(record.count));

    return recordNode;
}

std::string StatsFormatter::formatType(const StatsRecorder::Record::Type type) const
{
    return type == StatsRecorder::Record::Type::Layout ? "layout" : "media";
}

std::string StatsFormatter::formatMediaId(const boost::optional<int>& id) const
{
    return id ? std::to_string(id.value()) : "";
}
