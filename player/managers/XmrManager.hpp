#pragma once

#include "common/dt/DateTime.hpp"
#include "networking/ZeromqSubscriber.hpp"
#include "XmrStatus.hpp"

struct XmrMessage
{
    std::string action;
    DateTime createdDt;
    int ttl;
};

using CollectionIntervalAction = sigc::signal<void>;
using ScreenshotAction = sigc::signal<void>;

class XmrManager
{
public:
    void connect(const std::string& host);
    void stop();

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
