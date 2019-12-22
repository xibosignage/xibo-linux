#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

namespace SubmitLog
{
    struct Result
    {
        bool success;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> logXml{"logXml"};
    };
}

template <>
class Soap::RequestSerializer<SubmitLog::Request> : public BaseRequestSerializer<SubmitLog::Request>
{
public:
    RequestSerializer(const SubmitLog::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<SubmitLog::Result> : public BaseResponseParser<SubmitLog::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    SubmitLog::Result parseBody(const XmlNode& node) override;
};
