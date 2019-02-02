#include "HTTPManager.hpp"

#include "utils/Logger.hpp"

#include "Uri.hpp"
#include "UriParseError.hpp"
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
    m_ioc.stop();
}

boost::future<HTTPResponseResult> HTTPManager::get(const std::string& url)
{
    return send(http::verb::get, url, {});
}

boost::future<HTTPResponseResult> HTTPManager::post(const std::string& url, const std::string& body)
{
    return send(http::verb::post, url, body);
}

boost::future<HTTPResponseResult> HTTPManager::send(http::verb method, const Uri& uri, const std::string& body)
{
    auto session = std::make_shared<RequestSession>(m_ioc);
    return session->send(method, uri, body);
}
