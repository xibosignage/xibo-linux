#ifndef GETRESOURCE_HPP
#define GETRESOURCE_HPP

#include "Field.hpp"
#include "SOAP.hpp"

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
class SOAP::ResponseParser<GetResource::Response> : public BaseResponseParser
{
public:
    ResponseParser(const std::string& soapResponse);
    GetResource::Response get();

};

#endif // GETRESOURCE_HPP
