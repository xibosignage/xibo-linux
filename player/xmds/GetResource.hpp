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
        Field<std::string> server_key{"serverKey"};
        Field<std::string> hardware_key{"hardwareKey"};
        Field<int> layout_id{"layoutId"};
        Field<std::string> region_id{"regionId"};
        Field<std::string> media_id{"mediaId"};
    };
}

template<>
struct soap::request_traits<GetResource::Request>
{
    static inline const std::string name = "GetResource";
    using response_t = GetResource::Response;
};

#endif // GETRESOURCE_HPP
