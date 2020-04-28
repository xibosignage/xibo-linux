#pragma once

#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

struct GetResource
{
    struct Response
    {
        std::string resource;
    };

    struct Request : SoapRequest
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<int> layoutId{"layoutId"};
        SoapField<std::string> regionId{"regionId"};
        SoapField<std::string> mediaId{"mediaId"};
    };
};

template <>
class Soap::ResponseParser<GetResource::Response> : public BaseResponseParser<GetResource::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    GetResource::Response parseBody(const XmlNode& node) override;
};
