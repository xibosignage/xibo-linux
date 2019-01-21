#include "HTTPManager.hpp"

#include "utils/Logger.hpp"

#include <regex>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/asio/connect.hpp>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

const std::regex URL_REGEX("([A-Za-z]*://)(.*)(/.*)");
const int DEFAULT_PORT = 80;

struct RequestSession
{
    RequestSession(boost::asio::io_context& ioc) : socket(ioc), resolver(ioc)
    {
        httpResponse.body_limit(std::numeric_limits<std::uint64_t>::max());
    }

    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver resolver;
    boost::beast::http::request<boost::beast::http::string_body> httpRequest;
    boost::beast::http::response_parser<boost::beast::http::string_body> httpResponse;
    boost::beast::flat_buffer buffer;
    std::string target;
    std::string host;
    RequestFinishedCallback callback;
    std::promise<void> result;
};

HTTPManager::HTTPManager() :
    m_work{m_ioc}
{
    m_workThread = std::make_unique<JoinableThread>([=](){
        m_ioc.run();
    });
}

HTTPManager::~HTTPManager()
{
    m_ioc.stop();
}

std::future<void> HTTPManager::send(const std::string& url, RequestFinishedCallback callback)
{
    auto session = std::make_shared<RequestSession>(m_ioc);
    session->callback = callback;

    const int GROUPS_COUNT = 3;
    std::smatch baseMatch;

    if(std::regex_match(url, baseMatch, URL_REGEX) && baseMatch.size() > GROUPS_COUNT)
    {
        session->host = baseMatch[2].str();
        session->target = baseMatch[3].str();
        Log::trace("Host: {} Target: {}", session->host, session->target);
    }

    auto resolve = std::bind(&HTTPManager::onResolve, this, std::placeholders::_1, std::placeholders::_2, session);
    session->resolver.async_resolve(session->host, std::to_string(DEFAULT_PORT), ip::resolver_base::numeric_service, resolve);

    return session->result.get_future();
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

void HTTPManager::onWrite(const boost::system::error_code& ec, std::size_t, RequestSessionPtr session)
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
        session->httpRequest.method(http::verb::get);
        session->httpRequest.target(session->target);
        session->httpRequest.version(11);
        session->httpRequest.set(http::field::host, session->host);
        session->httpRequest.prepare_payload();

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
