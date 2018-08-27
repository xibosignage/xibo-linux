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
        std::string status_message;
        PlayerSettings player_settings;
    };

    struct Request
    {
        Field<std::string> server_key{"serverKey"};
        Field<std::string> hardware_key{"hardwareKey"};
        Field<std::string> display_name{"displayName"};
        Field<std::string> client_type{"clientType"};
        Field<std::string> client_version{"clientVersion"};
        Field<int> client_code{"clientCode"};
        Field<std::string> mac_address{"macAddress"};
    };
}

template<>
struct soap::request_traits<RegisterDisplay::Request>
{
    static inline const std::string name = "RegisterDisplay";
    using response_t = RegisterDisplay::Response;
};

#endif // REGISTERDISPLAY_HPP
