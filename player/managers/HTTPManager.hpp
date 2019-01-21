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

class HTTPManager
{
public:
    HTTPManager();
    ~HTTPManager();
    HTTPManager(const HTTPManager&) = delete;
    HTTPManager& operator=(const HTTPManager&) = delete;

    std::future<void> send(const std::string& url, RequestFinishedCallback callback);

private:
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
