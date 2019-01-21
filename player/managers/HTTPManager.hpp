#pragma once

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <future>
#include "utils/JoinableThread.hpp"

struct ResponseResult
{
    boost::system::error_code error;
    std::string result;
};

struct RequestSession;
using RequestSessionPtr = std::shared_ptr<RequestSession>;
using RequestFinishedCallback = std::function<void(const ResponseResult&)>;

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

class HTTPManager
{
public:
    HTTPManager();
    ~HTTPManager();
    HTTPManager(const HTTPManager&) = delete;
    HTTPManager& operator=(const HTTPManager&) = delete;

    std::future<void> get(const std::string& url, RequestFinishedCallback callback);
    std::future<void> post(const std::string& url, const std::string& body, RequestFinishedCallback callback);

private:
    std::future<void> send(http::verb method, const std::string& url, const std::string& body, RequestFinishedCallback callback);
    std::pair<std::string, std::string> parseUrl(const std::string& url);
    http::request<http::string_body> createRequest(http::verb method, const std::string& host, const std::string& target);

    void sessionFinished(const boost::system::error_code& ec, RequestSessionPtr session);
    void onRead(const boost::system::error_code& ec, std::size_t bytes_transferred, RequestSessionPtr session);
    void onWrite(const boost::system::error_code& ec, std::size_t bytes_transferred, RequestSessionPtr session);
    void onConnect(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator, RequestSessionPtr session);
    void onResolve(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::results_type results, RequestSessionPtr session);

private:
    boost::asio::io_context m_ioc;
    boost::asio::io_context::work m_work;
    std::unique_ptr<JoinableThread> m_workThread;
};
