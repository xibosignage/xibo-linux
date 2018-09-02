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
    XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    void registerDisplay(int clientCode,
                          const std::string& clientVersion,
                          const std::string& displayName,
                          RegisterDisplayCallback callback);
    void requiredFiles(RequiredFilesCallback callback);
    void getResource(int layoutId, int regionId, int mediaId, GetResourceCallback callback);

private:
    SOAPManager m_soapManager;
    std::string m_serverKey;
    std::string m_hardwareKey;
    std::shared_ptr<spdlog::logger> m_logger;

};

#endif // XMDSMANAGER_H
