#include "XmrSubscriber.hpp"

#include "constants.hpp"
#include "common/logger/Logging.hpp"
#include "common/Utils.hpp"

const size_t CHANNEL_PART = 0;
const size_t KEY_PART = 1;
const size_t MESSAGE_PART = 2;

XmrSubscriber::XmrSubscriber() :
    m_context{1}, m_socket{m_context, ZMQ_SUB}
{
}

void XmrSubscriber::connect(const std::string& host)
{
    if(m_host == host) return;

    m_host = host;
    m_socket.connect(host);
    m_socket.setsockopt(ZMQ_SUBSCRIBE, HEARTBEAT_CHANNEL, std::strlen(HEARTBEAT_CHANNEL));
    m_socket.setsockopt(ZMQ_SUBSCRIBE, XMR_CHANNEL, std::strlen(XMR_CHANNEL));

    m_stopped = false;
    m_worker = std::make_unique<JoinableThread>([this](){
        processMessageQueue();
    });

    Log::info("Connected to XMR publisher");
}

void XmrSubscriber::stop()
{
    m_stopped = true;
    m_socket.close();
    m_context.close();
    m_worker.reset();
}

CollectionIntervalAction& XmrSubscriber::collectionInterval()
{
    return m_collectionIntervalAction;
}

ScreenshotAction& XmrSubscriber::screenshot()
{
    return m_screenshotAction;
}

void XmrSubscriber::processMessageQueue()
{
    while(!m_stopped)
    {
        try
        {
            auto message = recvAll(m_socket);
            if(message[CHANNEL_PART] == HEARTBEAT_CHANNEL)
            {
                Log::trace("hearbeat");
            }
            else if(message[CHANNEL_PART] == XMR_CHANNEL)
            {
                auto xmrMessage = parseMessage(decryptMessage(message[KEY_PART], message[MESSAGE_PART]));
                processMessage(xmrMessage);
            }
        }
        catch(const zmq::error_t& ex)
        {
            if(ex.num() != ETERM)
                throw;
        }
    }
}

std::vector<std::string> XmrSubscriber::recvAll(zmq::socket_t& socket)
{
    std::vector<std::string> composedMessage;

    while(1)
    {
        zmq::message_t message;

        socket.recv(&message);
        std::string data{static_cast<char*>(message.data()), message.size()};
        composedMessage.emplace_back(data);

        if(!message.more())
            break;
    }

    return composedMessage;
}

XmrMessage XmrSubscriber::parseMessage(const std::string& jsonMessage)
{
    Log::debug(jsonMessage);
    auto tree = Utils::parseJsonFromString(jsonMessage);

    boost::posix_time::time_input_facet *tif = new boost::posix_time::time_input_facet;
    tif->set_iso_extended_format();
    std::istringstream iss(tree.get<std::string>("createdDt"));

    XmrMessage message;
    message.action = tree.get<std::string>("action");
    iss >> message.createdDt;
    message.ttl = tree.get<int>("ttl");

    return message;
}

bool XmrSubscriber::isMessageExpired(const XmrMessage& message)
{
    auto resultDt = message.createdDt + boost::posix_time::seconds(message.ttl);
    if(resultDt < boost::posix_time::second_clock::universal_time())
    {
        return true;
    }
    return false;
}

#include "common/RsaManager.hpp"
#include <boost/beast/core/detail/base64.hpp>
#include <cryptopp/arc4.h>

std::string XmrSubscriber::decryptMessage(const std::string& key, const std::string& message)
{
    auto k = boost::beast::detail::base64_decode(key);
    auto m = boost::beast::detail::base64_decode(message);

    std::string result;
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSAES_PKCS1v15_Decryptor d(RsaManager::instance().privateKey());
    CryptoPP::StringSource( k, true, new CryptoPP::PK_DecryptorFilter(rng, d, new CryptoPP::StringSink(result)));

    CryptoPP::ARC4 rc4((unsigned char*)result.c_str(), result.size());
    rc4.ProcessString((unsigned char*)m.c_str(), m.size());

    return m;
}

void XmrSubscriber::processMessage(const XmrMessage& message)
{
    if(isMessageExpired(message)) return;

    if(message.action == "collectNow")
    {
        m_collectionIntervalAction.emit();
    }
    else if(message.action == "screenShot")
    {
        m_screenshotAction.emit();
    }
}
