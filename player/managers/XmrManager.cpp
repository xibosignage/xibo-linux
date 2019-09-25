#include "XmrManager.hpp"

#include "constants.hpp"

#include "common/Parsing.hpp"
#include "common/Utils.hpp"
#include "common/crypto/RsaManager.hpp"
#include "common/dt/DateTime.hpp"
#include "common/logger/Logging.hpp"

const size_t CHANNEL_PART = 0;
const size_t KEY_PART = 1;
const size_t MESSAGE_PART = 2;

XmrManager::~XmrManager()
{
    m_subcriber.stop();
}

void XmrManager::connect(const std::string& host)
{
    if (m_info.host == host) return;

    m_info.host = host;
    m_subcriber.messageReceived().connect(
        [this](const MultiPartMessage& message) { processMultipartMessage(message); });
    m_subcriber.run(host);

    Log::info("Connected to XMR publisher");
}

CollectionIntervalAction& XmrManager::collectionInterval()
{
    return m_collectionIntervalAction;
}

ScreenshotAction& XmrManager::screenshot()
{
    return m_screenshotAction;
}

XmrStatus XmrManager::status()
{
    return m_info;
}

void XmrManager::processMultipartMessage(const MultiPartMessage& multipart)
{
    if (multipart[CHANNEL_PART] == XmrChannel)
    {
        try
        {
            auto decryptedMessage = decryptMessage(multipart[KEY_PART], multipart[MESSAGE_PART]);
            auto xmrMessage = parseMessage(decryptedMessage);

            processXmrMessage(xmrMessage);

            m_info.lastMessageDt = DateTime::now();
        }
        catch (std::exception& e)
        {
            Log::error("[XMR] {}", e.what());
        }
    }
    else
    {
        m_info.lastHeartbeatDt = DateTime::now();
    }
}

std::string XmrManager::decryptMessage(const std::string& encryptedBase64Key, const std::string& encryptedBase64Message)
{
    auto privateKey = RsaManager::instance().privateKey();

    auto encryptedKey = Utils::fromBase64(encryptedBase64Key);
    auto messageKey = CryptoUtils::decryptPrivateKeyPkcs(encryptedKey, privateKey);

    auto encryptedMessage = Utils::fromBase64(encryptedBase64Message);

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
        m_collectionIntervalAction();
    }
    else if (message.action == "screenShot")
    {
        m_screenshotAction();
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
