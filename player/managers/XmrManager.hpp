#pragma once

#include "common/dt/DateTime.hpp"
#include "managers/XmrStatus.hpp"
#include "networking/ZeromqSubscriber.hpp"

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
    ~XmrManager();

    void connect(const std::string& host);

    CollectionIntervalAction& collectionInterval();
    ScreenshotAction& screenshot();
    XmrStatus status();

private:
    void processMultipartMessage(const MultiPartMessage& message);
    std::string decryptMessage(const std::string& key, const std::string& message);
    XmrMessage parseMessage(const std::string& jsonMessage);
    void processXmrMessage(const XmrMessage& message);
    bool isMessageExpired(const XmrMessage& message);

private:
    ZeromqSubscriber subcriber_;
    CollectionIntervalAction collectionIntervalAction_;
    ScreenshotAction screenshotAction_;
    XmrStatus info_;
};
