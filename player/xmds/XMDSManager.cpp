#include "XMDSManager.hpp"
#include "SOAPManager.hpp"
#include "Resources.hpp"

const std::string DEFAULT_CLIENT_TYPE = "linux";

XMDSManager::XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey) :
    m_soapManager(std::make_unique<SOAPManager>(host)), m_serverKey(serverKey), m_hardwareKey(hardwareKey)
{
}

std::future<RegisterDisplay::Result> XMDSManager::registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName)
{
    RegisterDisplay::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.clientType = DEFAULT_CLIENT_TYPE;
    request.clientCode = clientCode;
    request.clientVersion = clientVersion;
    request.macAddress = "test";
    request.displayName = displayName;

    return m_soapManager->sendRequest<RegisterDisplay::Result>(request);
}

std::future<RequiredFiles::Result> XMDSManager::requiredFiles()
{
    RequiredFiles::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;

    return m_soapManager->sendRequest<RequiredFiles::Result>(request);
}

std::future<Schedule::Result> XMDSManager::schedule()
{
    Schedule::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;

    return m_soapManager->sendRequest<Schedule::Result>(request);
}

std::future<GetResource::Result> XMDSManager::getResource(int layoutId, int regionId, int mediaId)
{
    GetResource::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.layoutId = layoutId;
    request.regionId = std::to_string(regionId);
    request.mediaId = std::to_string(mediaId);

    return m_soapManager->sendRequest<GetResource::Result>(request);
}
