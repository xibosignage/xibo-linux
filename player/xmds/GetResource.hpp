#ifndef GETRESOURCE_HPP
#define GETRESOURCE_HPP

#include "field.hpp"
#include "soap.hpp"

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
struct soap::request_traits<GetResource::Request>
{
    static inline const std::string name = "GetResource";
    using response_t = GetResource::Response;
};

#endif // GETRESOURCE_HPP
