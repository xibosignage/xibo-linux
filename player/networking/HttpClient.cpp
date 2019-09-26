#include "HttpClient.hpp"

#include "common/logger/Logging.hpp"
#include "common/uri/Uri.hpp"
#include "common/uri/UriParseError.hpp"

#include "HttpRequest.hpp"
#include "HttpSession.hpp"
#include "ProxyHttpRequest.hpp"

const int DefaultConcurrentRequests = 4;

HttpClient::HttpClient() : m_work{m_ioc}
{
    for (int i = 0; i != DefaultConcurrentRequests; ++i)
    {
        m_workerThreads.push_back(std::make_unique<JoinableThread>([=]() {
            Log::trace("HTTP thread started");

            m_ioc.run();
        }));
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
    if (!m_ioc.stopped())
    {
        m_ioc.stop();
        cancelActiveSession();
    }
}

void HttpClient::setProxyServer(const std::string& host, const std::string& username, const std::string& password)
{
    if (!host.empty())
    {
        m_proxyInfo = ProxyInfo{host, username, password};
    }
}

void HttpClient::cancelActiveSession()
{
    for (auto&& session : m_activeSessions)
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
    if (m_ioc.stopped()) return managerStoppedError();

    auto session = std::make_shared<HttpSession>(m_ioc);
    m_activeSessions.push_back(session);

    Log::trace(uri);

    if (m_proxyInfo)
    {
        ProxyHttpRequest request{method, m_proxyInfo.value(), uri, body};
        return session->send(request.hostInfo(), request.get());
    }
    else
    {
        HttpRequest request{method, uri, body};
        return session->send(request.hostInfo(), request.get());
    }
}

boost::future<HttpResponseResult> HttpClient::managerStoppedError()
{
    boost::promise<HttpResponseResult> result;
    result.set_value(HttpResponseResult{PlayerError{PlayerError::Type::HTTP, "Manager Stopped"}, {}});
    return result.get_future();
}
