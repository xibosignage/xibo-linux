#ifndef NOTIFYSTATUS_HPP
#define NOTIFYSTATUS_HPP

#include "field.hpp"
#include "soap.hpp"

namespace NotifyStatus
{
    struct response
    {
        bool success;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
        field<std::string> status{"status"};
    };
}

template<>
struct soap::request_traits<NotifyStatus::request>
{
    static inline const std::string name = "NotifyStatus";
    using response_t = NotifyStatus::response;
};

#endif // NOTIFYSTATUS_HPP
