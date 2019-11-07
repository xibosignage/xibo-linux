#pragma once

#include "common/JoinableThread.hpp"
#include "xmr/zmq/Context.hpp"
#include "xmr/zmq/Socket.hpp"

#include <boost/signals2/signal.hpp>

namespace Zmq
{
    using MessageReceived = boost::signals2::signal<void(const MultiPartMessage&)>;

    class Subscriber
    {
    public:
        ~Subscriber();

        void run(const std::string& host, const Channels& channels);
        void stop();

        MessageReceived& messageReceived();

    private:
        void runMessageQueue(Socket& socket);

    private:
        std::unique_ptr<JoinableThread> worker_;
        MessageReceived messageReceived_;
        std::unique_ptr<Context> context_;
    };
}
