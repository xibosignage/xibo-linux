#pragma once

#include <string>
#include <zmq.hpp>
#include <atomic>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <glibmm/dispatcher.h>

#include "common/JoinableThread.hpp"

struct XmrMessage
{
    std::string action;
    boost::posix_time::ptime createdDt;
    int ttl;
};

using CollectionIntervalAction = Glib::Dispatcher;
using ScreenshotAction = Glib::Dispatcher;

class XmrSubscriber
{
public:
    XmrSubscriber();

    void connect(const std::string& host);
    void stop();

    CollectionIntervalAction& collectionInterval();
    ScreenshotAction& screenshot();

private:
    void processMessageQueue();
    std::vector<std::string> recvAll(zmq::socket_t& socket);
    XmrMessage parseMessage(const std::string& jsonMessage);
    bool isMessageExpired(const XmrMessage& message);
    std::string decryptMessage(const std::string& key, const std::string& message);
    void processMessage(const XmrMessage& action);

private:
    std::unique_ptr<JoinableThread> m_worker;
    CollectionIntervalAction m_collectionIntervalAction;
    ScreenshotAction m_screenshotAction;
    std::atomic<bool> m_stopped;
    std::string m_host;
    zmq::context_t m_context;
    zmq::socket_t m_socket;

};
