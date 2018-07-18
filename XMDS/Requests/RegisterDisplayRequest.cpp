#include "RegisterDisplayRequest.hpp"

RegisterDisplayRequest::RegisterDisplayRequest() :
    BaseRequest("displayName", "clientType", "clientVersion", "clientCode", "macAddress")
{
}

std::string RegisterDisplayRequest::display_name() const
{
    return field<0>().value();
}

void RegisterDisplayRequest::display_name(const std::string& value)
{
    field<0>().set_value(value);
}

std::string RegisterDisplayRequest::client_type() const
{
    return field<1>().value();
}

void RegisterDisplayRequest::client_type(const std::string& value)
{
    field<1>().set_value(value);
}

std::string RegisterDisplayRequest::client_version() const
{
    return field<2>().value();
}

void RegisterDisplayRequest::client_version(const std::string& value)
{
    field<2>().set_value(value);
}

int RegisterDisplayRequest::client_code() const
{
    return field<3>().value();
}

void RegisterDisplayRequest::client_code(int value)
{
    field<3>().set_value(value);
}

std::string RegisterDisplayRequest::mac_address() const
{
    return field<4>().value();
}

void RegisterDisplayRequest::mac_address(const std::string& value)
{
    field<4>().set_value(value);
}
