#ifndef NOTIFYSTATUS_HPP
#define NOTIFYSTATUS_HPP

#include "field.hpp"
#include "soap.hpp"

namespace NotifyStatus
{
    struct Response
    {
        bool success;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
        Field<std::string> status{"status"};
    };
}

template<>
struct soap::request_traits<NotifyStatus::Request>
{
    static inline const std::string name = "NotifyStatus";
    using response_t = NotifyStatus::Response;
};

#endif // NOTIFYSTATUS_HPP
