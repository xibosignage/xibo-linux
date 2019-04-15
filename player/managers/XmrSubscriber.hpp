#pragma once

#include <string>
#include <zmq.hpp>
#include <atomic>

#include "common/JoinableThread.hpp"

class XmrSubscriber
{
public:
    XmrSubscriber();

    void connect(const std::string& host);
    void stop();

private:
    void processMessageQueue();
    std::vector<std::string> recvAll(zmq::socket_t& socket);

private:
    std::unique_ptr<JoinableThread> m_worker;
    std::atomic<bool> m_stopped;
    std::string m_host;
    zmq::context_t m_context;
    zmq::socket_t m_socket;

};
