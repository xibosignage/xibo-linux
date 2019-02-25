#include "HTTPManager.hpp"

#include "utils/Logger.hpp"
#include "utils/Uri.hpp"
#include "utils/UriParseError.hpp"

#include "HTTPSession.hpp"

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

HTTPManager::~HTTPManager()
{
    shutdown();
}

void HTTPManager::shutdown()
{
    if(!m_ioc.stopped())
    {
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

boost::future<HTTPResponseResult> HTTPManager::get(const std::string& uri)
{
    return send(http::verb::get, uri, {});
}

boost::future<HTTPResponseResult> HTTPManager::post(const std::string& uri, const std::string& body)
{
    return send(http::verb::post, uri, body);
}

boost::future<HTTPResponseResult> HTTPManager::send(http::verb method, const Uri& uri, const std::string& body)
{
    if(m_ioc.stopped()) return managerStoppedError();

    auto session = std::make_shared<HTTPSession>(m_ioc);
    m_activeSessions.push_back(session);

    Log::debug(uri);

    return session->send(method, uri, body);
}

boost::future<HTTPResponseResult> HTTPManager::managerStoppedError()
{
    boost::promise<HTTPResponseResult> result;
    result.set_value(HTTPResponseResult{PlayerError{PlayerError::Type::HTTP, "Manager Stopped"}, {}});
    return result.get_future();
}
