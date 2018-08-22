#include "RegisterDisplay.hpp"
#include "xmds.hpp"

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
        result.collect_interval = display.get<int>("collectInterval");
        result.download_start_window = display.get<std::string>("downloadStartWindow");
        result.download_end_window = display.get<std::string>("downloadEndWindow");
        result.stats_enabled = display.get<bool>("statsEnabled");
        result.xmr_network_address = display.get<std::string>("xmrNetworkAddress");
        result.sizeX = display.get<double>("sizeX");
        result.sizeY = display.get<double>("sizeY");
        result.offsetX = display.get<double>("offsetX");
        result.offsetY = display.get<double>("offsetY");
        result.log_level = display.get<std::string>("logLevel");
        result.shell_commands_enabled = display.get<bool>("enableShellCommands");
        result.modified_layouts_enabled = display.get<bool>("expireModifiedLayouts");
        result.max_concurrent_downloads = display.get<int>("maxConcurrentDownloads");
        //shellCommandAllowList
        result.status_layout_update = display.get<bool>("sendCurrentLayoutAsStatusUpdate");
        result.screenshot_interval = display.get<int>("screenShotRequestInterval");
        result.screenshot_size = display.get<int>("screenShotSize");
        result.max_log_files_uploads = display.get<int>("maxLogFileUploads");
        result.embedded_server_port = display.get<int>("embeddedServerPort");
        result.prevent_sleep = display.get<bool>("preventSleep");
        result.display_name = display.get<std::string>("displayName");
        result.screenshot_requested = display.get<bool>("screenShotRequested");
    }
    return result;
}
