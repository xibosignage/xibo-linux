#include "XMDSManager.hpp"

const std::string DEFAULT_CLIENT_TYPE = "linux";

XMDSManager::XMDSManager(const std::string &host, const std::string& server_key, const std::string& hardware_key) :
    m_soap_manager(host), m_server_key(server_key), m_hardware_key(hardware_key)
{
    m_logger = spdlog::get(LOGGER);
}

void XMDSManager::register_display(int client_code, const std::string& client_version,
                                   const std::string& display_name, RegisterDisplayCallback callback)
{
    RegisterDisplay::Request request;
    request.server_key = m_server_key;
    request.hardware_key = m_hardware_key;
    request.client_type = DEFAULT_CLIENT_TYPE;
    request.client_code = client_code;
    request.client_version = client_version; // FIXME get from player version (?)
    request.mac_address = "test"; // FIXME get from the system
    request.display_name = display_name;

    m_soap_manager.send_request(request, callback);
}

void XMDSManager::required_files(RequiredFilesCallback callback)
{
    RequiredFiles::Request request;
    request.server_key = m_server_key;
    request.hardware_key = m_hardware_key;

    m_soap_manager.send_request(request, callback);
}


void XMDSManager::get_resource(int layout_id, int region_id, int media_id, GetResourceCallback callback)
{
    GetResource::Request request;
    request.server_key = m_server_key;
    request.hardware_key = m_hardware_key;
    request.layout_id = layout_id;
    request.region_id = std::to_string(region_id);
    request.media_id = std::to_string(media_id);

    m_soap_manager.send_request(request, callback);
}
