#include "StatsRecorder.hpp"

#include "common/Parsing.hpp"

StatsRecorder& StatsRecorder::get()
{
    static StatsRecorder recorder;
    return recorder;
}

void StatsRecorder::addLayoutStat(int scheduleId, const LayoutStat& stat)
{
    long duration = (stat.finished - stat.started).seconds();
    Record record{"layout", stat.started, stat.finished, scheduleId, stat.id, {}, duration, 1};
    records.emplace_back(std::move(record));
}

void StatsRecorder::addMediaStats(int layoutId, int scheduleId, const std::vector<MediaStat>& mediaStats)
{
    for (auto&& stat : mediaStats)
    {
        long duration = (stat.finished - stat.started).seconds();
        Record record{"media", stat.started, stat.finished, scheduleId, layoutId, stat.id, duration, 1};
        records.emplace_back(std::move(record));
    }
}

std::string StatsRecorder::xmlString() const
{
    XmlNode root;
    auto&& stats = root.put_child("stats", XmlNode{});

    for (auto&& record : records)
    {
        XmlNode recordNode;

        recordNode.put(Parsing::xmlAttr("type"), record.type);
        recordNode.put(Parsing::xmlAttr("fromdt"), record.started.string("%Y-%m-%d %H:%M:%S"));
        recordNode.put(Parsing::xmlAttr("todt"), record.finished.string("%Y-%m-%d %H:%M:%S"));
        recordNode.put(Parsing::xmlAttr("scheduleid"), std::to_string(record.scheduleId));
        recordNode.put(Parsing::xmlAttr("layoutid"), std::to_string(record.layoutId));
        recordNode.put(Parsing::xmlAttr("mediaid"), std::to_string(record.mediaId));
        recordNode.put(Parsing::xmlAttr("duration"), std::to_string(record.duration));
        recordNode.put(Parsing::xmlAttr("count"), std::to_string(record.count));

        stats.put_child("stat", recordNode);
    }

    return Parsing::xmlTreeToString(root);
}
