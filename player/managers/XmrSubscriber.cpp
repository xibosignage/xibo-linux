#include "XmrSubscriber.hpp"

#include "constants.hpp"
#include "common/logger/Logging.hpp"
#include "common/Utils.hpp"

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
            for(auto&& part : message)
            {
                Log::debug("Part {}", part);
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
        composedMessage.emplace_back(static_cast<char*>(message.data()));

        if(!message.more())
            break;
    }

    return composedMessage;
}

XmrMessage XmrSubscriber::parseMessage(const std::string& jsonMessage)
{
    auto tree = Utils::parseJsonFromString(jsonMessage);

    XmrMessage message;
    message.action = tree.get<std::string>("action");
    message.createdDt = boost::posix_time::time_from_string(tree.get<std::string>("createdDt"));
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
