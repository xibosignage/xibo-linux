#include "XmrManager.hpp"

#include "MainLoop.hpp"

#include "common/parsing/Parsing.hpp"
#include "common/crypto/RsaManager.hpp"
#include "common/dt/DateTime.hpp"
#include "common/logger/Logging.hpp"
#include "config/AppConfig.hpp"

const size_t CHANNEL_PART = 0;
const size_t KEY_PART = 1;
const size_t MESSAGE_PART = 2;

const char* const HearbeatChannel = "H";

XmrManager::XmrManager(const XmrChannel& mainChannel) : mainChannel_(static_cast<std::string>(mainChannel)) {}

// TODO: strong type
void XmrManager::connect(const std::string& host)
{
    if (info_.host == host) return;

    info_.host = host;
    subscriber_.messageReceived().connect(
        [this](const Zmq::MultiPartMessage& message) { processMultipartMessage(message); });
    subscriber_.run(host, Zmq::Channels{mainChannel_, HearbeatChannel});
}

void XmrManager::stop()
{
    subscriber_.stop();
}

CollectionIntervalAction& XmrManager::collectionInterval()
{
    return collectionIntervalAction_;
}

ScreenshotAction& XmrManager::screenshot()
{
    return screenshotAction_;
}

XmrStatus XmrManager::status()
{
    return info_;
}

void XmrManager::processMultipartMessage(const Zmq::MultiPartMessage& multipart)
{
    if (multipart[CHANNEL_PART] == mainChannel_)
    {
        try
        {
            auto decryptedMessage = decryptMessage(multipart[KEY_PART], multipart[MESSAGE_PART]);
            auto xmrMessage = parseMessage(decryptedMessage);

            processXmrMessage(xmrMessage);

            info_.lastMessageDt = DateTime::now();
        }
        catch (CryptoPP::Exception& e)
        {
            Log::error("[XMR::Crypto] {}. You need to reconfigure XMR for this display in the CMS and wait for the "
                       "next collection "
                       "interval so all keys will be updated.",
                       e.what());
        }
        catch (std::exception& e)
        {
            Log::error("[XMR] {}", e.what());
        }
    }
    else
    {
        info_.lastHeartbeatDt = DateTime::now();
    }
}

std::string XmrManager::decryptMessage(const std::string& encryptedBase64Key, const std::string& encryptedBase64Message)
{
    auto privateKey = RsaManager::instance().privateKey();

    auto encryptedKey = CryptoUtils::fromBase64(encryptedBase64Key);
    auto messageKey = CryptoUtils::decryptPrivateKeyPkcs(encryptedKey, privateKey);

    auto encryptedMessage = CryptoUtils::fromBase64(encryptedBase64Message);

    return CryptoUtils::decryptRc4(encryptedMessage, messageKey);
}

XmrMessage XmrManager::parseMessage(const std::string& jsonMessage)
{
    auto tree = Parsing::jsonFromString(jsonMessage);

    XmrMessage message;
    message.action = tree.get<std::string>("action");
    message.createdDt = DateTime::fromIsoExtendedString(tree.get<std::string>("createdDt"));
    message.ttl = tree.get<int>("ttl");

    return message;
}

void XmrManager::processXmrMessage(const XmrMessage& message)
{
    if (isMessageExpired(message)) return;

    if (message.action == "collectNow")
    {
        MainLoop::pushToUiThread([this]() { collectionIntervalAction_(); });
    }
    else if (message.action == "screenShot")
    {
        MainLoop::pushToUiThread([this]() { screenshotAction_(); });
    }
}

bool XmrManager::isMessageExpired(const XmrMessage& message)
{
    auto resultDt = message.createdDt + DateTime::Seconds(message.ttl);
    if (resultDt < DateTime::nowUtc())
    {
        return true;
    }
    return false;
}
