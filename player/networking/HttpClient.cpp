#include "HttpClient.hpp"

#include "common/logger/Logging.hpp"
#include "common/types/Uri.hpp"

#include "networking/HttpRequest.hpp"
#include "networking/HttpSession.hpp"
#include "networking/ProxyHttpRequest.hpp"

const int DefaultConcurrentRequests = 4;

HttpClient::HttpClient() : work_{ioc_}
{
    for (int i = 0; i != DefaultConcurrentRequests; ++i)
    {
        workerThreads_.push_back(std::make_unique<JoinableThread>([=]() { ioc_.run(); }));
    }
}

HttpClient::~HttpClient()
{
    shutdown();
}

HttpClient& HttpClient::instance()
{
    static HttpClient manager;
    return manager;
}

void HttpClient::shutdown()
{
    if (!ioc_.stopped())
    {
        ioc_.stop();
        cancelActiveSession();
    }
}

// TODO: strong type
void HttpClient::setProxyServer(const std::string& host, const std::string& username, const std::string& password)
{
    if (!host.empty())
    {
        proxyInfo_ = ProxyInfo{host, username, password};
    }
}

void HttpClient::cancelActiveSession()
{
    for (auto&& session : activeSessions_)
    {
        if (!session.expired())
        {
            session.lock()->cancel();
        }
    }
}

boost::future<HttpResponseResult> HttpClient::get(const Uri& uri)
{
    return send(http::verb::get, uri, {});
}

boost::future<HttpResponseResult> HttpClient::post(const Uri& uri, const std::string& body)
{
    return send(http::verb::post, uri, body);
}

boost::future<HttpResponseResult> HttpClient::send(http::verb method, const Uri& uri, const std::string& body)
{
    if (ioc_.stopped()) return managerStoppedError();

    auto session = std::make_shared<HttpSession>(ioc_);
    activeSessions_.push_back(session);

    if (proxyInfo_)
    {
        ProxyHttpRequest request{method, proxyInfo_.value(), uri, body};
        return session->send(uri, request.get());
    }
    else
    {
        HttpRequest request{method, uri, body};
        return session->send(uri, request.get());
    }
}

boost::future<HttpResponseResult> HttpClient::managerStoppedError()
{
    boost::promise<HttpResponseResult> result;
    result.set_value(HttpResponseResult{PlayerError{"HTTP", "Manager Stopped"}, {}});
    return result.get_future();
}
