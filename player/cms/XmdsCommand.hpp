#pragma once

#include <boost/signals2/signal.hpp>
#include <boost/thread/future.hpp>

#include "cms/xmds/SoapRequestSender.hpp"

class IXmdsCommand
{
public:
    using SignalError = boost::signals2::signal<void(const PlayerError&)>;
    using SignalFinished = boost::signals2::signal<void()>;

    virtual ~IXmdsCommand() = default;
    virtual void execute() = 0;
    virtual SignalError& error() = 0;
    virtual SignalFinished& finished() = 0;
};

template <typename Call>
class XmdsCommand : public IXmdsCommand
{
    using Request = typename Call::Request;
    using Response = typename Call::Response;

    static const inline std::string XmdsTarget = "/xmds.php?v=5";

public:
    using FutureResponseResult = boost::future<std::pair<PlayerError, Response>>;

public:
    XmdsCommand(const std::string& host, const std::string& serverKey, const std::string& hardwareKey) :
        uri_(Uri::fromString(host + XmdsTarget)),
        serverKey_(serverKey),
        hardwareKey_(hardwareKey)
    {
    }

    void execute() override
    {
        namespace ph = std::placeholders;

        Request request;
        request.serverKey = serverKey_;
        request.hardwareKey = hardwareKey_;
        prepare(request);

        SoapRequestHelper::sendRequest<Response>(uri_, request)
            .then(std::bind(&XmdsCommand::onResponseReceived, this, ph::_1));
    }

    SignalFinished& finished() override
    {
        return finished_;
    }

    SignalError& error() override
    {
        return error_;
    }

protected:
    ~XmdsCommand() = default;

    virtual void prepare(Request&) {}
    virtual void process(const Response& response) = 0;

private:
    void onResponseReceived(FutureResponseResult futureResult)
    {
        auto [error, response] = futureResult.get();
        if (!error)
        {
            process(response);
        }
        else
        {
            error_(error);
        }
    }

private:
    Uri uri_;
    std::string serverKey_;
    std::string hardwareKey_;
    SignalFinished finished_;
    SignalError error_;
};
