#ifndef GETRESOURCE_HPP
#define GETRESOURCE_HPP

#include "field.hpp"
#include "soap.hpp"

namespace GetResource
{
    struct response
    {
        std::string resource;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
        field<int> layout_id{"layoutId"};
        field<std::string> region_id{"regionId"};
        field<std::string> media_id{"mediaId"};
    };
}

template<>
struct soap::request_traits<GetResource::request>
{
    static inline const std::string name = "GetResource";
    using response_t = GetResource::response;
};

#endif // GETRESOURCE_HPP
