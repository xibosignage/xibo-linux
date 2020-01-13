#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

namespace SubmitStats
{
    struct Result
    {
        bool success;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> statXml{"statXml"};
    };
}

template <>
class Soap::RequestSerializer<SubmitStats::Request> : public BaseRequestSerializer<SubmitStats::Request>
{
public:
    RequestSerializer(const SubmitStats::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<SubmitStats::Result> : public BaseResponseParser<SubmitStats::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    SubmitStats::Result parseBody(const XmlNode& node) override;
};
