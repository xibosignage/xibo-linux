#pragma once

#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

struct SubmitScreenshot
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
        SoapField<std::string> screenShot{"screenShot"};
    };
};

template <>
class Soap::ResponseParser<SubmitScreenshot::Response> : public BaseResponseParser<SubmitScreenshot::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    SubmitScreenshot::Response parseBody(const XmlNode& node) override;
};
