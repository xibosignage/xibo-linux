#ifndef XMDSMANAGER_H
#define XMDSMANAGER_H

#include "SOAPManager.hpp"

#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "GetResource.hpp"

#include <spdlog/spdlog.h>

using RegisterDisplayCallback = std::function<void(const RegisterDisplay::Response&)>;
using RequiredFilesCallback = std::function<void(const RequiredFiles::Response&)>;
using GetResourceCallback = std::function<void(const GetResource::Response&)>;

class XMDSManager
{
public:
    XMDSManager(const std::string& host, const std::string& server_key, const std::string& hardware_key);

    void register_display(int client_code,
                          const std::string& client_version,
                          const std::string& display_name,
                          RegisterDisplayCallback callback);
    void required_files(RequiredFilesCallback callback);
    void get_resource(int layout_id, int region_id, int media_id, GetResourceCallback callback);

private:
    SOAPManager m_soap_manager;
    std::string m_server_key;
    std::string m_hardware_key;
    std::shared_ptr<spdlog::logger> m_logger;

};

#endif // XMDSMANAGER_H
