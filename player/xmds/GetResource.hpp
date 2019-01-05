#pragma once

#include "Field.hpp"
#include "SOAP.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

namespace GetResource
{
    struct Response
    {
        std::string resource;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
        Field<int> layoutId{"layoutId"};
        Field<std::string> regionId{"regionId"};
        Field<std::string> mediaId{"mediaId"};
    };
}

template<>
class SOAP::RequestSerializer<GetResource::Request> : public BaseRequestSerializer<GetResource::Request>
{
public:
    RequestSerializer(const GetResource::Request& request);
    std::string string();

};

template<>
class SOAP::ResponseParser<GetResource::Response> : public BaseResponseParser<GetResource::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    GetResource::Response doParse(const boost::property_tree::ptree& node) override;

};
