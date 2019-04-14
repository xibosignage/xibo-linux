#include "XmdsRequestSender.hpp"
#include "SoapRequestSender.hpp"
#include "Resources.hpp"

#include "common/MacAddressFetcher.hpp"

const std::string DEFAULT_CLIENT_TYPE = "linux";
const std::string UNDEFINED_MAC_ADDRESS = "00:00:00:00:00:00";
const std::string XMDS_TARGET = "/xmds.php?v=5";

XmdsRequestSender::XmdsRequestSender(const std::string& host, const std::string& serverKey, const std::string& hardwareKey) :
    m_uri(host + XMDS_TARGET),
    m_serverKey(serverKey),
    m_hardwareKey(hardwareKey)
{
}

boost::future<ResponseResult<RegisterDisplay::Result>> XmdsRequestSender::registerDisplay(int clientCode, const std::string& clientVersion, const std::string& displayName)
{
    RegisterDisplay::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.clientType = DEFAULT_CLIENT_TYPE;
    request.clientCode = clientCode;
    request.clientVersion = clientVersion;
    request.macAddress = MacAddressFetcher::get().value_or(UNDEFINED_MAC_ADDRESS);
    request.displayName = displayName;

    return SoapRequestHelper::sendRequest<RegisterDisplay::Result>(m_uri, request);
}

boost::future<ResponseResult<RequiredFiles::Result>> XmdsRequestSender::requiredFiles()
{
    RequiredFiles::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;

    return SoapRequestHelper::sendRequest<RequiredFiles::Result>(m_uri, request);
}

boost::future<ResponseResult<Schedule::Result>> XmdsRequestSender::schedule()
{
    Schedule::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;

    return SoapRequestHelper::sendRequest<Schedule::Result>(m_uri, request);
}

boost::future<ResponseResult<GetResource::Result>> XmdsRequestSender::getResource(int layoutId, int regionId, int mediaId)
{
    GetResource::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.layoutId = layoutId;
    request.regionId = std::to_string(regionId);
    request.mediaId = std::to_string(mediaId);

    return SoapRequestHelper::sendRequest<GetResource::Result>(m_uri, request);
}

boost::future<ResponseResult<GetFile::Result>> XmdsRequestSender::getFile(int fileId, const std::string& fileType, std::size_t chunkOffset, std::size_t chunkSize)
{
    GetFile::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.fileId = std::to_string(fileId);
    request.fileType = fileType;
    request.chunkOffset = chunkOffset;
    request.chunkSize = chunkSize;

    return SoapRequestHelper::sendRequest<GetFile::Result>(m_uri, request);
}

boost::future<ResponseResult<MediaInventory::Result>> XmdsRequestSender::mediaInventory(MediaInventoryItems&& items)
{
    MediaInventory::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.inventory = std::move(items);

    return SoapRequestHelper::sendRequest<MediaInventory::Result>(m_uri, request);
}

boost::future<ResponseResult<SubmitLog::Result>> XmdsRequestSender::submitLogs(const std::string& logXml)
{
    SubmitLog::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.logXml = logXml;

    return SoapRequestHelper::sendRequest<SubmitLog::Result>(m_uri, request);
}

boost::future<ResponseResult<SubmitScreenShot::Result>> XmdsRequestSender::submitScreenShot(const std::string& screenShot)
{
    SubmitScreenShot::Request request;
    request.serverKey = m_serverKey;
    request.hardwareKey = m_hardwareKey;
    request.screenShot = screenShot;

    return SoapRequestHelper::sendRequest<SubmitScreenShot::Result>(m_uri, request);
}
