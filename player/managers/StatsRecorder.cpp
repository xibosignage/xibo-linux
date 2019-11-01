#include "StatsRecorder.hpp"

#include "common/Parsing.hpp"

void StatsRecorder::addLayoutStat(int scheduleId, const LayoutStat& stat)
{
    long duration = (stat.finished - stat.started).total_seconds();
    Record record{"layout", stat.started, stat.finished, scheduleId, stat.id, {}, duration, 1};
    records_.emplace_back(std::move(record));
}

void StatsRecorder::addMediaStats(int layoutId, int scheduleId, const std::vector<MediaStat>& mediaStats)
{
    for (auto&& stat : mediaStats)
    {
        long duration = (stat.finished - stat.started).total_seconds();
        Record record{"media", stat.started, stat.finished, scheduleId, layoutId, stat.id, duration, 1};
        records_.emplace_back(std::move(record));
    }
}

void StatsRecorder::clear()
{
    records_.clear();
}

std::string StatsRecorder::toXml() const
{
    XmlNode root;
    auto&& stats = root.put_child("stats", XmlNode{});

    for (auto&& record : records_)
    {
        XmlNode recordNode;

        recordNode.put(Parsing::xmlAttr("type"), record.type);
        recordNode.put(Parsing::xmlAttr("fromdt"), record.started.string("%Y-%m-%d %H:%M:%S"));
        recordNode.put(Parsing::xmlAttr("todt"), record.finished.string("%Y-%m-%d %H:%M:%S"));
        recordNode.put(Parsing::xmlAttr("scheduleid"), std::to_string(record.scheduleId));
        recordNode.put(Parsing::xmlAttr("layoutid"), std::to_string(record.layoutId));
        recordNode.put(Parsing::xmlAttr("mediaid"), record.mediaId != 0 ? std::to_string(record.mediaId) : "");
        recordNode.put(Parsing::xmlAttr("duration"), std::to_string(record.duration));
        recordNode.put(Parsing::xmlAttr("count"), std::to_string(record.count));

        stats.add_child("stat", recordNode);
    }

    return Parsing::xmlTreeToString(root);
}
