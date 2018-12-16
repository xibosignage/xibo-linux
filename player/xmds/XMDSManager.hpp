#ifndef XMDSMANAGER_H
#define XMDSMANAGER_H

#include "RegisterDisplay.hpp"
#include "RequiredFiles.hpp"
#include "GetResource.hpp"

class SOAPManager;

using RegisterDisplayCallback = std::function<void(RegisterDisplay::Response::Status, const PlayerSettings&)>;
using RequiredFilesCallback = std::function<void(const RegularFiles&, const ResourceFiles&)>;
using GetResourceCallback = std::function<void(const GetResource::Response&)>;

class XMDSManager
{
public:
    XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey);

    void registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName, RegisterDisplayCallback callback);
    void requiredFiles(RequiredFilesCallback callback);
    void getResource(int layoutId, int regionId, int mediaId, GetResourceCallback callback);

private:
    std::unique_ptr<SOAPManager> m_soapManager;
    std::string m_serverKey;
    std::string m_hardwareKey;

};

#endif // XMDSMANAGER_H
