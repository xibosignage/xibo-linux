#include "ZeromqSubscriber.hpp"

#include "common/logger/Logging.hpp"
#include "constants.hpp"

ZeromqSubscriber::ZeromqSubscriber() : stopped_(true) {}

void ZeromqSubscriber::stop()
{
    if (!stopped_)
    {
        stopped_ = true;
        if (context_)
        {
            context_->close();
        }
        worker_.reset();
    }
}

MessageReceived& ZeromqSubscriber::messageReceived()
{
    return messageReceived_;
}

void ZeromqSubscriber::run(const std::string& host, const Channels& channels)
{
    stop();
    worker_ = std::make_unique<JoinableThread>(
        [this, host, channels = std::move(channels)]() { processMessageQueue(host, channels); });
}

bool ZeromqSubscriber::init()
{
    try
    {
        context_ = std::make_unique<zmq::context_t>(1);
        stopped_ = false;

        return true;
    }
    catch (zmq::error_t& ex)
    {
        Log::error("[ZMQ] Init error {}", ex.what());
    }

    return false;
}

void ZeromqSubscriber::processMessageQueue(const std::string& host, const Channels& channels)
{
    if (!init()) return;

    zmq::socket_t socket{*context_, ZMQ_SUB};
    tryConnect(socket, host, channels);

    Log::info("[ZMQ] Successful connection");

    while (!stopped_)
    {
        try
        {
            messageReceived_(recvAll(socket));
        }
        catch (const zmq::error_t& ex)
        {
            if (ex.num() != ETERM)
            {
                Log::error("[ZMQ] Receive message error: {}", ex.what());
            }
        }
    }
}

void ZeromqSubscriber::tryConnect(zmq::socket_t& socket, const std::string& host, const Channels& channels)
{
    try
    {
        socket.connect(host);
        for (auto&& channel : channels)
        {
            socket.setsockopt(ZMQ_SUBSCRIBE, channel.c_str(), channel.size());
        }
    }
    catch (zmq::error_t& ex)
    {
        stopped_ = true;
        Log::error("[ZMQ] Unsuccesful connection to {}. Reason: {}", host, ex.what());
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
