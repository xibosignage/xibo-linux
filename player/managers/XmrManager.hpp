#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>

#include "networking/ZeromqSubscriber.hpp"
#include "XmrStatus.hpp"

struct XmrMessage
{
    std::string action;
    boost::posix_time::ptime createdDt;
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
    boost::posix_time::ptime parseDateTime(const std::string& dt);
    void processXmrMessage(const XmrMessage& message);
    bool isMessageExpired(const XmrMessage& message);

private:
    ZeromqSubscriber m_subcriber;
    CollectionIntervalAction m_collectionIntervalAction;
    ScreenshotAction m_screenshotAction;
    XmrStatus m_info;

};
