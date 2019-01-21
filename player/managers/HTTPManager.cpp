#include "HTTPManager.hpp"

#include "utils/Logger.hpp"

#include <regex>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/asio/connect.hpp>

const std::regex URL_REGEX("([A-Za-z]*://)(.*)(/.*)");
const int DEFAULT_HTTP_VERSION = 11;
const int DEFAULT_PORT = 80;
const int DEFAULT_CONCURRENT_REQUESTS = 2;

struct RequestSession
{
    RequestSession(asio::io_context& ioc) : socket(ioc), resolver(ioc)
    {
        httpResponse.body_limit(std::numeric_limits<std::uint64_t>::max());
    }

    ip::tcp::socket socket;
    ip::tcp::resolver resolver;
    http::request<http::string_body> httpRequest;
    http::response_parser<http::string_body> httpResponse;
    beast::flat_buffer buffer;
    RequestFinishedCallback callback;
    std::promise<void> result;
};

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

std::future<void> HTTPManager::get(const std::string& url, RequestFinishedCallback callback)
{
    return send(http::verb::get, url, {}, callback);
}

std::future<void> HTTPManager::post(const std::string& url, const std::string& body, RequestFinishedCallback callback)
{
    return send(http::verb::post, url, body, callback);
}

std::future<void> HTTPManager::send(http::verb method, const std::string& url, const std::string& body, RequestFinishedCallback callback)
{
    auto [host, target] = parseUrl(url);
    auto session = std::make_shared<RequestSession>(m_ioc);

    session->callback = callback;
    session->httpRequest = createRequest(method, host, target);
    session->httpRequest.body() = body;
    session->httpRequest.prepare_payload();

    auto resolve = std::bind(&HTTPManager::onResolve, this, std::placeholders::_1, std::placeholders::_2, session);
    session->resolver.async_resolve(host, std::to_string(DEFAULT_PORT), ip::resolver_base::numeric_service, resolve);

    return session->result.get_future();
}

// FIXME use strong_typdef
std::pair<std::string, std::string> HTTPManager::parseUrl(const std::string& url)
{
    const int GROUPS_COUNT = 3;
    std::smatch baseMatch;
    std::string host, target;

    if(std::regex_match(url, baseMatch, URL_REGEX) && baseMatch.size() > GROUPS_COUNT)
    {
        host = baseMatch[2].str();
        target = baseMatch[3].str();
    }

    return std::pair{host, target};
}

boost::beast::http::request<http::string_body> HTTPManager::createRequest(http::verb method, const std::string& host, const std::string& target)
{
    boost::beast::http::request<http::string_body> request;
    request.method(method);
    request.target(target);
    request.version(DEFAULT_HTTP_VERSION);
    request.set(http::field::host, host);
    return request;
}

void HTTPManager::sessionFinished(const boost::system::error_code& ec, RequestSessionPtr session)
{
    session->callback(ResponseResult{ec, session->httpResponse.get().body()});
    session->result.set_value();
}

void HTTPManager::onRead(const boost::system::error_code& ec, std::size_t /*bytes*/, RequestSessionPtr session)
{
    sessionFinished(ec, session);
}

void HTTPManager::onWrite(const boost::system::error_code& ec, std::size_t /*bytes*/, RequestSessionPtr session)
{
    if(!ec)
    {  
        auto read = std::bind(&HTTPManager::onRead, this, std::placeholders::_1, std::placeholders::_2, session);
        http::async_read(session->socket, session->buffer, session->httpResponse, read);
    }
    else
    {
        sessionFinished(ec, session);
    }
}

void HTTPManager::onConnect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, RequestSessionPtr session)
{
    if(!ec)
    {
        auto write = std::bind(&HTTPManager::onWrite, this, std::placeholders::_1, std::placeholders::_2, session);
        http::async_write(session->socket, session->httpRequest, write);
    }
    else
    {
        sessionFinished(ec, session);
    }
}

void HTTPManager::onResolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, RequestSessionPtr session)
{
    if(!ec)
    {
        auto connect = std::bind(&HTTPManager::onConnect, this, std::placeholders::_1, std::placeholders::_2, session);
        asio::async_connect(session->socket, results.begin(), results.end(), connect);
    }
    else
    {
        sessionFinished(ec, session);
    }
}
