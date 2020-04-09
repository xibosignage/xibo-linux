#pragma once

#include <boost/signals2/signal.hpp>
#include <boost/thread/future.hpp>

#include "cms/xmds/SoapRequestSender.hpp"

template <typename Call>
class XmdsCommand
{
    using SignalFinished = boost::signals2::signal<void()>;
    using SignalError = boost::signals2::signal<void(const PlayerError&)>;

    using Request = typename Call::Request;
    using Response = typename Call::Response;

    static const inline std::string XmdsTarget = "/xmds.php?v=5";

public:
    using FutureResponseResult = boost::future<std::pair<PlayerError, Response>>;

public:
    XmdsCommand(const std::string& host) : uri_(Uri::fromString(host + XmdsTarget)) {}

    void execute()
    {
        namespace ph = std::placeholders;

        auto request = prepareRequest();
        SoapRequestHelper::sendRequest<Response>(uri_, request)
            .then(std::bind(&XmdsCommand::onResponseReceived, this, ph::_1));
    }

    SignalFinished& finished()
    {
        return finished_;
    }

    SignalError& error()
    {
        return error_;
    }

protected:
    ~XmdsCommand() = default;

    virtual Request prepareRequest() = 0;
    virtual void processResponse(const Response& response) = 0;

private:
    void onResponseReceived(FutureResponseResult futureResult)
    {
        auto [error, response] = futureResult.get();
        if (!error)
        {
            processResponse(response);
        }
        else
        {
            error_(error);
        }
    }

private:
    Uri uri_;
    SignalFinished finished_;
    SignalError error_;
};
