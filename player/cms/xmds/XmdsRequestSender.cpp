#include "XmdsRequestSender.hpp"

#include "cms/xmds/Resources.hpp"
#include "cms/xmds/SoapRequestSender.hpp"

#include "common/crypto/RsaManager.hpp"
#include "common/system/System.hpp"
#include "xmr/XmrChannel.hpp"

const std::string DefaultClientType = "linux";
const std::string XmdsTarget = "/xmds.php?v=5";

XmdsRequestSender::XmdsRequestSender(const std::string& host,
                                     const std::string& serverKey,
                                     const std::string& hardwareKey) :
    uri_(Uri::fromString(host + XmdsTarget)),
    host_(host),
    serverKey_(serverKey),
    hardwareKey_(hardwareKey)
{
}

FutureResponseResult<RegisterDisplay::Result> XmdsRequestSender::registerDisplay(const std::string& clientCode,
                                                                                 const std::string& clientVersion,
                                                                                 const std::string& displayName)
{
    RegisterDisplay::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.clientType = DefaultClientType;
    request.clientCode = clientCode;
    request.clientVersion = clientVersion;
    request.macAddress = static_cast<std::string>(System::macAddress());
    request.xmrChannel = static_cast<std::string>(XmrChannel::fromCmsSettings(host_, serverKey_, hardwareKey_));
    request.xmrPubKey = CryptoUtils::keyToString(RsaManager::instance().publicKey());
    request.displayName = displayName;

    return SoapRequestHelper::sendRequest<RegisterDisplay::Result>(uri_, request);
}

FutureResponseResult<RequiredFiles::Result> XmdsRequestSender::requiredFiles()
{
    RequiredFiles::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;

    return SoapRequestHelper::sendRequest<RequiredFiles::Result>(uri_, request);
}

FutureResponseResult<Schedule::Result> XmdsRequestSender::schedule()
{
    Schedule::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;

    return SoapRequestHelper::sendRequest<Schedule::Result>(uri_, request);
}

FutureResponseResult<GetResource::Result> XmdsRequestSender::getResource(int layoutId, int regionId, int mediaId)
{
    GetResource::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.layoutId = layoutId;
    request.regionId = std::to_string(regionId);
    request.mediaId = std::to_string(mediaId);

    return SoapRequestHelper::sendRequest<GetResource::Result>(uri_, request);
}

FutureResponseResult<GetFile::Result> XmdsRequestSender::getFile(int fileId,
                                                                 const std::string& fileType,
                                                                 std::size_t chunkOffset,
                                                                 std::size_t chunkSize)
{
    GetFile::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.fileId = std::to_string(fileId);
    request.fileType = fileType;
    request.chunkOffset = chunkOffset;
    request.chunkSize = chunkSize;

    return SoapRequestHelper::sendRequest<GetFile::Result>(uri_, request);
}

FutureResponseResult<MediaInventory::Result> XmdsRequestSender::mediaInventory(MediaInventoryItems&& inventory)
{
    MediaInventory::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.inventory = std::move(inventory);

    return SoapRequestHelper::sendRequest<MediaInventory::Result>(uri_, request);
}

FutureResponseResult<SubmitLog::Result> XmdsRequestSender::submitLogs(const std::string& logXml)
{
    SubmitLog::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.logXml = std::string("<![CDATA[") + logXml + "]]>";

    return SoapRequestHelper::sendRequest<SubmitLog::Result>(uri_, request);
}

FutureResponseResult<SubmitStats::Result> XmdsRequestSender::submitStats(const std::string& statXml)
{
    SubmitStats::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.statXml = std::string("<![CDATA[") + statXml + "]]>";

    return SoapRequestHelper::sendRequest<SubmitStats::Result>(uri_, request);
}

FutureResponseResult<SubmitScreenShot::Result> XmdsRequestSender::submitScreenShot(const std::string& screenShot)
{
    SubmitScreenShot::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.screenShot = screenShot;

    return SoapRequestHelper::sendRequest<SubmitScreenShot::Result>(uri_, request);
}

FutureResponseResult<NotifyStatus::Result> XmdsRequestSender::notifyStatus(const std::string& status)
{
    NotifyStatus::Request request;
    request.serverKey = serverKey_;
    request.hardwareKey = hardwareKey_;
    request.status = status;

    return SoapRequestHelper::sendRequest<NotifyStatus::Result>(uri_, request);
}
