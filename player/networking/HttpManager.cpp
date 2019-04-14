#include "HttpManager.hpp"

#include "common/logger/Logging.hpp"
#include "common/uri/Uri.hpp"
#include "common/uri/UriParseError.hpp"

#include "HttpSession.hpp"

const int DEFAULT_CONCURRENT_REQUESTS = 4;

HttpManager::HttpManager() :
    m_work{m_ioc}
{
    for(int i = 0; i != DEFAULT_CONCURRENT_REQUESTS; ++i)
    {
        m_workerThreads.push_back(std::make_unique<JoinableThread>([=](){
            Log::trace("HTTP thread started");

            m_ioc.run();
        }));
    }
}

HttpManager::~HttpManager()
{
    shutdown();
}

HttpManager& HttpManager::instance()
{
    static HttpManager manager;
    return manager;
}

void HttpManager::shutdown()
{
    if(!m_ioc.stopped())
    {
        m_ioc.stop();
        cancelActiveSession();
    }
}

void HttpManager::cancelActiveSession()
{
    for(auto&& session : m_activeSessions)
    {
        if(!session.expired())
        {
            session.lock()->cancel();
        }
    }
}

boost::future<HttpResponseResult> HttpManager::get(const Uri& uri)
{
    return send(http::verb::get, uri, {});
}

boost::future<HttpResponseResult> HttpManager::post(const Uri& uri, const std::string& body)
{
    return send(http::verb::post, uri, body);
}

boost::future<HttpResponseResult> HttpManager::send(http::verb method, const Uri& uri, const std::string& body)
{
    if(m_ioc.stopped()) return managerStoppedError();

    auto session = std::make_shared<HttpSession>(m_ioc);
    m_activeSessions.push_back(session);

    Log::debug(uri);

    return session->send(method, uri, body);
}

boost::future<HttpResponseResult> HttpManager::managerStoppedError()
{
    boost::promise<HttpResponseResult> result;
    result.set_value(HttpResponseResult{PlayerError{PlayerError::Type::HTTP, "Manager Stopped"}, {}});
    return result.get_future();
}
