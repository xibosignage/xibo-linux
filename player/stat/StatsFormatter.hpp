#pragma once

#include "common/Parsing.hpp"
#include "stat/StatsRecorder.hpp"

class StatsFormatter
{
public:
    std::string toXml(const StatsRecorder::Records& records) const;

private:
    XmlNode createRecordNode(const StatsRecorder::Record& record) const;
    std::string formatType(const StatsRecorder::Record::Type type) const;
    std::string formatMediaId(const boost::optional<int>& id) const;
};
