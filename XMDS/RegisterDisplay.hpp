#ifndef REGISTERDISPLAY_HPP
#define REGISTERDISPLAY_HPP

#include "field.hpp"
#include "soap.hpp"

namespace RegisterDisplay
{
    struct response
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
        int collect_interval;
        std::string download_start_window;
        std::string download_end_window;
        bool stats_enabled;
        std::string xmr_network_address;
        double sizeX;
        double sizeY;
        double offsetX;
        double offsetY;
        std::string log_level;
        bool shell_commands_enabled;
        bool modified_layouts_enabled;
        int max_concurrent_downloads;
        bool status_layout_update;
        int screenshot_interval;
        int screenshot_size;
        int max_log_files_uploads;
        int embedded_server_port;
        bool prevent_sleep;
        std::string display_name;
        bool screenshot_requested;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
        field<std::string> display_name{"displayName"};
        field<std::string> client_type{"clientType"};
        field<std::string> client_version{"clientVersion"};
        field<int> client_code{"clientCode"};
        field<std::string> mac_address{"macAddress"};
    };
}

template<>
struct soap::request_traits<RegisterDisplay::request>
{
    static inline const std::string name = "RegisterDisplay";
    using response_t = RegisterDisplay::response;
};

#endif // REGISTERDISPLAY_HPP
