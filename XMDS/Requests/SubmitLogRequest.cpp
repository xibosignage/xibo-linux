#include "SubmitLogRequest.hpp"

SubmitLogRequest::SubmitLogRequest() : BaseRequest("logXml")
{
}

std::string SubmitLogRequest::log_xml() const
{
    return field<0>().value();
}

void SubmitLogRequest::log_xml(const std::string& value)
{
    field<0>().set_value(value);
}
