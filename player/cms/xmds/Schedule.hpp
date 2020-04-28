#pragma once

#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

struct Schedule
{
    struct Response
    {
        std::string scheduleXml;
    };

    struct Request : SoapRequest
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
    };
};

template <>
class Soap::ResponseParser<Schedule::Response> : public BaseResponseParser<Schedule::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    Schedule::Response parseBody(const XmlNode& scheduleNode) override;
};
