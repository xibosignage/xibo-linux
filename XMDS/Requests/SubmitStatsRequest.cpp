#include "SubmitStatsRequest.hpp"

SubmitStatsRequest::SubmitStatsRequest() : BaseRequest("statXml")
{
}

std::string SubmitStatsRequest::stat_xml() const
{
    return field<0>().value();
}

void SubmitStatsRequest::stat_xml(const std::string& value)
{
    field<0>().set_value(value);
}
