#ifndef REGISTERDISPLAY_HPP
#define REGISTERDISPLAY_HPP

#include "field.hpp"
#include "soap.hpp"
#include "control/PlayerSettings.hpp"

namespace RegisterDisplay
{
    struct Response
    {
        enum class Status
        {
            Ready,
            Added,
            Waiting,
            Invalid = -1
        };

        Status status = Status::Invalid;
        std::string statusMessage;
        PlayerSettings playerSettings;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
        Field<std::string> displayName{"displayName"};
        Field<std::string> clientType{"clientType"};
        Field<std::string> clientVersion{"clientVersion"};
        Field<int> clientCode{"clientCode"};
        Field<std::string> macAddress{"macAddress"};
    };
}

template<>
struct soap::request_traits<RegisterDisplay::Request>
{
    static inline const std::string name = "RegisterDisplay";
    using response_t = RegisterDisplay::Response;
};

#endif // REGISTERDISPLAY_HPP
