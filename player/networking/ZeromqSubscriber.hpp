#pragma once

#include <atomic>
#include <zmq.hpp>

#include "common/Dispatcher.hpp"
#include "common/JoinableThread.hpp"

using MultiPartMessage = std::vector<std::string>;
using MessageReceived = Dispatcher<MultiPartMessage>;

class ZeromqSubscriber
{
public:
    ZeromqSubscriber();

    void run(const std::string& host);
    void stop();

    MessageReceived& messageReceived();

private:
    void processMessageQueue(const std::string& host);
    void tryConnect(zmq::socket_t& socket, const std::string& host);
    MultiPartMessage recvAll(zmq::socket_t& socket);

private:
    std::unique_ptr<JoinableThread> worker_;
    MessageReceived messageReceived_;
    std::atomic<bool> stopped_ = false;
    zmq::context_t context_;
};
