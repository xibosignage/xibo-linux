#pragma once

#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

struct SubmitStats
{
    struct Response
    {
        bool success;
    };

    struct Request : SoapRequest
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> statXml{"statXml"};
    };
};

template <>
class Soap::ResponseParser<SubmitStats::Response> : public BaseResponseParser<SubmitStats::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    SubmitStats::Response parseBody(const XmlNode& node) override;
};
