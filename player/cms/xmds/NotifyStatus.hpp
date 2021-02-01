#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

namespace NotifyStatus
{
    struct Result
    {
        bool success;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> status{"status"};
    };
}

template <>
class Soap::RequestSerializer<NotifyStatus::Request> : public BaseRequestSerializer<NotifyStatus::Request>
{
public:
    RequestSerializer(const NotifyStatus::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<NotifyStatus::Result> : public BaseResponseParser<NotifyStatus::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    NotifyStatus::Result parseBody(const XmlNode& node) override;
};
