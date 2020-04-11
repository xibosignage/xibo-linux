#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

struct SubmitScreenshot
{
    struct Response
    {
        bool success;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> screenShot{"screenShot"};
    };
};

template <>
class Soap::RequestSerializer<SubmitScreenshot::Request> : public BaseRequestSerializer<SubmitScreenshot::Request>
{
public:
    RequestSerializer(const SubmitScreenshot::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<SubmitScreenshot::Response> : public BaseResponseParser<SubmitScreenshot::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    SubmitScreenshot::Response parseBody(const XmlNode& node) override;
};
