#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

namespace SubmitScreenShot
{
    struct Result
    {
        bool success;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> screenShot{"screenShot"};
    };
}

template <>
class Soap::RequestSerializer<SubmitScreenShot::Request> : public BaseRequestSerializer<SubmitScreenShot::Request>
{
public:
    RequestSerializer(const SubmitScreenShot::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<SubmitScreenShot::Result> : public BaseResponseParser<SubmitScreenShot::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    SubmitScreenShot::Result parseBody(const XmlNode& node) override;
};
