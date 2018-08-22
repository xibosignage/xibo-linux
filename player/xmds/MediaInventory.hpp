#ifndef MEDIAINVENTORY_HPP
#define MEDIAINVENTORY_HPP

#include "field.hpp"
#include "soap.hpp"

namespace MediaInventory
{
    struct Response
    {
        bool success;
    };

    struct Request
    {
        Field<std::string> server_key{"serverKey"};
        Field<std::string> hardware_key{"hardwareKey"};
        Field<std::string> media_inventory{"mediaInventory"};
    };
}

template<>
struct soap::request_traits<MediaInventory::Request>
{
    static inline const std::string name = "MediaInventory";
    using response_t = MediaInventory::Response;
};

#endif // MEDIAINVENTORY_HPP
