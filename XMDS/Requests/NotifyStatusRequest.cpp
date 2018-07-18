#include "NotifyStatusRequest.hpp"

NotifyStatusRequest::NotifyStatusRequest() : BaseRequest("status")
{
}

std::string NotifyStatusRequest::status() const
{
    return field<0>().value();
}

void NotifyStatusRequest::status(const std::string& value)
{
    field<0>().set_value(value);
}
