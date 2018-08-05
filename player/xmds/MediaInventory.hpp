#ifndef MEDIAINVENTORY_HPP
#define MEDIAINVENTORY_HPP

#include "field.hpp"
#include "soap.hpp"

namespace MediaInventory
{
    struct response
    {
        bool success;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
        field<std::string> media_inventory{"mediaInventory"};
    };
}

template<>
struct soap::request_traits<MediaInventory::request>
{
    static inline const std::string name = "MediaInventory";
    using response_t = MediaInventory::response;
};

#endif // MEDIAINVENTORY_HPP
