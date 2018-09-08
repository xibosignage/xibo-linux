#include "XMDSManager.hpp"

const std::string DEFAULT_CLIENT_TYPE = "linux";

XMDSManager::XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey) :
    m_soapManager(host), m_serverKey(serverKey), m_hardwareKey(hardwareKey)
{
    m_logger = spdlog::get(LOGGER);
}

void XMDSManager::registerDisplay(int clientCode, const std::string& clientVersion,
                                   const std::string& displayName, RegisterDisplayCallback callback)
{
    RegisterDisplay::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.clientType = DEFAULT_CLIENT_TYPE;
    request.clientCode = clientCode;
    request.clientVersion = clientVersion; // WARNING get from player version (?)
    request.macAddress = "test"; // FIXME get from the system
    request.displayName = displayName;

    m_soapManager.sendRequest(request, callback);
}

void XMDSManager::requiredFiles(RequiredFilesCallback callback)
{
    RequiredFiles::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;

    m_soapManager.sendRequest(request, callback);
}


void XMDSManager::getResource(int layoutId, int regionId, int mediaId, GetResourceCallback callback)
{
    GetResource::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.layoutId = layoutId;
    request.regionId = std::to_string(regionId);
    request.mediaId = std::to_string(mediaId);

    m_soapManager.sendRequest(request, callback);
}
