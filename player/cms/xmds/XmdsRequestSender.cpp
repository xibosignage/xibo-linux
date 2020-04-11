#include "XmdsRequestSender.hpp"

#include "cms/xmds/Resources.hpp"
#include "cms/xmds/SoapRequestSender.hpp"

const std::string XmdsTarget = "/xmds.php?v=5";

XmdsRequestSender::XmdsRequestSender(const std::string& host,
                                     const std::string& serverKey,
                                     const std::string& hardwareKey) :
    uri_(Uri::fromString(host + XmdsTarget)),
    host_(host),
    serverKey_(serverKey),
    hardwareKey_(hardwareKey)
{
}

std::string XmdsRequestSender::getHost() const
{
    return host_;
}

std::string XmdsRequestSender::getServerKey() const
{
    return serverKey_;
}

std::string XmdsRequestSender::getHardwareKey() const
{
    return hardwareKey_;
}
