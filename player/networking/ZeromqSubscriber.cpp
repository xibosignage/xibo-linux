#include "ZeromqSubscriber.hpp"

#include "common/logger/Logging.hpp"
#include "constants.hpp"

const char* const HeartbeatChannel = "H";

ZeromqSubscriber::ZeromqSubscriber() : context_{1} {}

void ZeromqSubscriber::stop()
{
    stopped_ = true;
    context_.close();
    worker_.reset();
}

MessageReceived& ZeromqSubscriber::messageReceived()
{
    return messageReceived_;
}

void ZeromqSubscriber::run(const std::string& host)
{
    worker_ = std::make_unique<JoinableThread>([this, host]() { processMessageQueue(host); });
}

void ZeromqSubscriber::processMessageQueue(const std::string& host)
{
    zmq::socket_t socket{context_, ZMQ_SUB};
    tryConnect(socket, host);
    socket.setsockopt(ZMQ_SUBSCRIBE, HeartbeatChannel, std::strlen(HeartbeatChannel));
    socket.setsockopt(ZMQ_SUBSCRIBE, XmrChannel, std::strlen(XmrChannel));

    while (!stopped_)
    {
        try
        {
            messageReceived_.emit(recvAll(socket));
        }
        catch (const zmq::error_t& ex)
        {
            if (ex.num() != ETERM) throw;
        }
    }
}

void ZeromqSubscriber::tryConnect(zmq::socket_t& socket, const std::string& host)
{
    try
    {
        socket.connect(host);
    }
    catch (zmq::error_t&)
    {
        Log::error("[ZeroMQ] Unsuccesful connection to {}", host);
    }
}

std::vector<std::string> ZeromqSubscriber::recvAll(zmq::socket_t& socket)
{
    std::vector<std::string> composedMessage;

    while (1)
    {
        zmq::message_t message;

        socket.recv(&message);
        std::string data{static_cast<char*>(message.data()), message.size()};
        composedMessage.emplace_back(data);

        if (!message.more()) break;
    }

    return composedMessage;
}
