#include "RegisterDisplay.hpp"

#include "xmds.hpp"
#include "utils/utilities.hpp"
#include "control/PlayerSettings.hpp"

template<>
std::string soap::request_string(const RegisterDisplay::Request& request)
{
    return create_request<RegisterDisplay::Request>
            (request.client_code, request.client_type, request.client_version,
             request.display_name, request.mac_address, request.server_key, request.hardware_key);
}

template<>
RegisterDisplay::Response soap::create_response(const std::string& soap_response)
{
    auto display = xmds::parse_xml_response(soap_response).get_child("display");
    auto attrs = display.get_child("<xmlattr>");

    RegisterDisplay::Response result;
    result.status = static_cast<RegisterDisplay::Response::Status>(attrs.get<int>("status"));
    result.status_message = attrs.get<std::string>("message");
    if(result.status == RegisterDisplay::Response::Status::Ready)
    {
        result.player_settings.collect_interval = display.get<int>("collectInterval");
        result.player_settings.download_start_window = display.get<std::string>("downloadStartWindow");
        result.player_settings.download_end_window = display.get<std::string>("downloadEndWindow");
        result.player_settings.stats_enabled = display.get<bool>("statsEnabled");
        result.player_settings.xmr_network_address = display.get<std::string>("xmrNetworkAddress");
        result.player_settings.sizeX = display.get<double>("sizeX");
        result.player_settings.sizeY = display.get<double>("sizeY");
        result.player_settings.offsetX = display.get<double>("offsetX");
        result.player_settings.offsetY = display.get<double>("offsetY");
        result.player_settings.log_level = display.get<std::string>("logLevel");
        result.player_settings.shell_commands_enabled = display.get<bool>("enableShellCommands");
        result.player_settings.modified_layouts_enabled = display.get<bool>("expireModifiedLayouts");
        result.player_settings.max_concurrent_downloads = display.get<int>("maxConcurrentDownloads");
        //shellCommandAllowList
        result.player_settings.status_layout_update = display.get<bool>("sendCurrentLayoutAsStatusUpdate");
        result.player_settings.screenshot_interval = display.get<int>("screenShotRequestInterval");
        result.player_settings.screenshot_size = display.get<int>("screenShotSize");
        result.player_settings.max_log_files_uploads = display.get<int>("maxLogFileUploads");
        result.player_settings.embedded_server_port = display.get<int>("embeddedServerPort");
        result.player_settings.prevent_sleep = display.get<bool>("preventSleep");
        result.player_settings.display_name = display.get<std::string>("displayName");
        result.player_settings.screenshot_requested = display.get<bool>("screenShotRequested");
    }
    return result;
}
