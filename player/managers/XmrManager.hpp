#pragma once

#include "XmrStatus.hpp"
#include "common/dt/DateTime.hpp"
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
    ZeromqSubscriber m_subcriber;
    CollectionIntervalAction m_collectionIntervalAction;
    ScreenshotAction m_screenshotAction;
    XmrStatus m_info;
};
