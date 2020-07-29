#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

namespace Schedule
{
    struct Result
    {
        std::string scheduleXml;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
    };
}

template <>
class Soap::RequestSerializer<Schedule::Request> : public BaseRequestSerializer<Schedule::Request>
{
public:
    RequestSerializer(const Schedule::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<Schedule::Result> : public BaseResponseParser<Schedule::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    Schedule::Result parseBody(const XmlNode& scheduleNode) override;
};
