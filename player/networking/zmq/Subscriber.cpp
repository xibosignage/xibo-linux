#include "Subscriber.hpp"

#include "common/logger/Logging.hpp"
#include "networking/zmq/Exception.hpp"

Zmq::Subscriber::Subscriber() {}

Zmq::Subscriber::~Subscriber()
{
    stop();
}

void Zmq::Subscriber::stop()
{
    if (context_ && context_->running())
    {
        context_->shutdown();   // We must shutdown any pending blocking operations
        worker_.reset();        // We join message queue thread and close all sockets on that thread
        context_->terminate();  // We should terminate context only after all sockets has been closed
    }
}

MessageReceived& Zmq::Subscriber::messageReceived()
{
    return messageReceived_;
}

void Zmq::Subscriber::run(const std::string& host, const Channels& channels)
{
    stop();
    worker_ = std::make_unique<JoinableThread>([this, host, channels = std::move(channels)]() {
        context_ = std::make_unique<Context>();
        Socket socket{*context_};
        socket.connect(host, channels);

        runMessageQueue(socket);
    });
}

void Zmq::Subscriber::runMessageQueue(Zmq::Socket& socket)
{
    assert(context_);

    while (context_->running())
    {
        try
        {
            messageReceived_(socket.receiveAll());
        }
        catch (Zmq::Exception& e)
        {
            if (e.code() != ETERM)
            {
                Log::error("[ZMQ] Receive message error: {}", e.what());
            }
        }
    }
}
