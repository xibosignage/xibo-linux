#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

struct Schedule
{
    struct Response
    {
        std::string scheduleXml;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
    };
};

template <>
class Soap::RequestSerializer<Schedule::Request> : public BaseRequestSerializer<Schedule::Request>
{
public:
    RequestSerializer(const Schedule::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<Schedule::Response> : public BaseResponseParser<Schedule::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    Schedule::Response parseBody(const XmlNode& scheduleNode) override;
};
