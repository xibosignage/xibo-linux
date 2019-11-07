#include "Subscriber.hpp"
#include "common/logger/Logging.hpp"
#include "xmr/zmq/Exception.hpp"

#include <zmq.h>

Zmq::Subscriber::~Subscriber()
{
    // check that thread has been joined and context has been terminated
    assert(!worker_);
    assert(context_ ? !context_->running() : true);
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

Zmq::MessageReceived& Zmq::Subscriber::messageReceived()
{
    return messageReceived_;
}

void Zmq::Subscriber::run(const std::string& host, const Channels& channels)
{
    stop();  // Stop previous connection if needed
    worker_ = std::make_unique<JoinableThread>([this, host, channels = std::move(channels)]() {
        try
        {
            context_ = std::make_unique<Context>();
            Socket socket{*context_};  // socket should be created on message queue thread
            socket.connect(host, channels);

            runMessageQueue(socket);
        }
        catch (std::exception& e)
        {
            Log::error("[ZMQ] {}", e.what());
        }
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
