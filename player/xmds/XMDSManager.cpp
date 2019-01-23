#include "XMDSManager.hpp"
#include "SOAPManager.hpp"
#include "Resources.hpp"

const std::string DEFAULT_CLIENT_TYPE = "linux";

XMDSManager::XMDSManager(const std::string& host, const std::string& serverKey, const std::string& hardwareKey) :
    m_soapManager(std::make_unique<SOAPManager>(host)), m_serverKey(serverKey), m_hardwareKey(hardwareKey)
{
}

boost::future<ResponseResult<RegisterDisplay::Result>> XMDSManager::registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName)
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

boost::future<ResponseResult<RequiredFiles::Result>> XMDSManager::requiredFiles()
{
    RequiredFiles::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;

    return m_soapManager->sendRequest<RequiredFiles::Result>(request);
}

boost::future<ResponseResult<Schedule::Result>> XMDSManager::schedule()
{
    Schedule::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;

    return m_soapManager->sendRequest<Schedule::Result>(request);
}

boost::future<ResponseResult<GetResource::Result>> XMDSManager::getResource(int layoutId, int regionId, int mediaId)
{
    GetResource::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.layoutId = layoutId;
    request.regionId = std::to_string(regionId);
    request.mediaId = std::to_string(mediaId);

    return m_soapManager->sendRequest<GetResource::Result>(request);
}

boost::future<ResponseResult<GetFile::Result>> XMDSManager::getFile(int fileId, const std::string& fileType, std::size_t chunkOffset, std::size_t chunkSize)
{
    GetFile::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.fileId = std::to_string(fileId);
    request.fileType = fileType;
    request.chunkOffset = chunkOffset;
    request.chunkSize = chunkSize;

    return m_soapManager->sendRequest<GetFile::Result>(request);
}
