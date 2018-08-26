#ifndef GETRESOURSE_HPP
#define GETRESOURSE_HPP

#include "field.hpp"
#include "soap.hpp"

namespace GetResourse
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
struct soap::request_traits<GetResourse::request>
{
    static inline const std::string name = "GetResourse";
    using response_t = GetResourse::response;
};

#endif // GETRESOURSE_HPP
