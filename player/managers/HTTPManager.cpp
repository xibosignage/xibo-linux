#include "HTTPManager.hpp"

#include "utils/Logger.hpp"
#include "HTTPSession.hpp"
#include "UrlParser.hpp"

const int DEFAULT_CONCURRENT_REQUESTS = 4;

HTTPManager::HTTPManager() :
    m_work{m_ioc}
{
    for(int i = 0; i != DEFAULT_CONCURRENT_REQUESTS; ++i)
    {
        m_workerThreads.push_back(std::make_unique<JoinableThread>([=](){
            Log::trace("HTTP thread {}", std::this_thread::get_id());

            m_ioc.run();
        }));
    }
}

void HTTPManager::shutdown()
{
    if(!m_ioc.stopped())
    {
        Log::debug("HTTPManager shutdown");
        m_ioc.stop();
        cancelActiveSession();
    }
}

void HTTPManager::cancelActiveSession()
{
    for(auto&& session : m_activeSessions)
    {
        if(!session.expired())
        {
            session.lock()->cancel();
        }
    }
}

boost::future<HTTPResponseResult> HTTPManager::get(const std::string& url)
{
    return send(http::verb::get, UrlParser{}.parse(url), {});
}

boost::future<HTTPResponseResult> HTTPManager::post(const std::string& url, const std::string& body)
{
    return send(http::verb::post, UrlParser{}.parse(url), body);
}

boost::future<HTTPResponseResult> HTTPManager::send(http::verb method, const Url& url, const std::string& body)
{
    if(m_ioc.stopped()) return managerStoppedError();

    auto session = std::make_shared<HTTPSession>(m_ioc);
    m_activeSessions.push_back(session);

    url.print();

    return session->send(method, url, body);
}

boost::future<HTTPResponseResult> HTTPManager::managerStoppedError()
{
    boost::promise<HTTPResponseResult> result;
    result.set_value(HTTPResponseResult{PlayerError{PlayerError::Type::HTTP, "Manager Stopped"}, {}});
    return result.get_future();
}
