#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

namespace GetResource
{
    struct Result
    {
        std::string resource;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<int> layoutId{"layoutId"};
        SoapField<std::string> regionId{"regionId"};
        SoapField<std::string> mediaId{"mediaId"};
    };
}

template <>
class Soap::RequestSerializer<GetResource::Request> : public BaseRequestSerializer<GetResource::Request>
{
public:
    RequestSerializer(const GetResource::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<GetResource::Result> : public BaseResponseParser<GetResource::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    GetResource::Result parseBody(const XmlNode& node) override;
};
