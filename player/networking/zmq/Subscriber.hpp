#pragma once

#include <boost/signals2/signal.hpp>

#include "common/JoinableThread.hpp"
#include "networking/zmq/Context.hpp"
#include "networking/zmq/Socket.hpp"

using MessageReceived = boost::signals2::signal<void(const MultiPartMessage&)>;

namespace Zmq
{
    class Subscriber
    {
    public:
        struct Error : std::runtime_error
        {
            using std::runtime_error::runtime_error;
        };

        Subscriber();
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
