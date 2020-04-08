#pragma once

#include "common/dt/DateTime.hpp"
#include "xmr/XmrChannel.hpp"
#include "xmr/XmrStatus.hpp"
#include "xmr/zmq/Subscriber.hpp"

#include <boost/signals2/signal.hpp>

struct XmrMessage
{
    std::string action;
    DateTime createdDt;
    int ttl;
};

using CollectionIntervalAction = boost::signals2::signal<void()>;
using ScreenshotAction = boost::signals2::signal<void()>;

class XmrManager
{
public:
    XmrManager(const XmrChannel& mainChannel);

    void connect(const std::string& host);
    void stop();

    CollectionIntervalAction& collectionInterval();
    ScreenshotAction& screenshot();
    XmrStatus status();

private:
    void processMultipartMessage(const Zmq::MultiPartMessage& message);
    std::string decryptMessage(const std::string& key, const std::string& message);
    XmrMessage parseMessage(const std::string& jsonMessage);
    void processXmrMessage(const XmrMessage& message);
    bool isMessageExpired(const XmrMessage& message);

private:
    std::string mainChannel_;
    Zmq::Subscriber subscriber_;
    CollectionIntervalAction collectionIntervalAction_;
    ScreenshotAction screenshotAction_;
    XmrStatus info_;
};
