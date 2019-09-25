#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>

#include <boost/thread/future.hpp>

#include "HostInfo.hpp"
#include "ResponseResult.hpp"

namespace http = boost::beast::http;
namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;

using HttpResponseResult = ResponseResult<std::string>;

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:
    HttpSession(boost::asio::io_context& ioc);

    boost::future<HttpResponseResult> send(const HostInfo& info, const http::request<http::string_body>& request);
    void cancel();

private:
    void sessionFinished(const boost::system::error_code& ec);
    void setHttpResult(const HttpResponseResult& result);

    template <typename Callback>
    void resolve(const std::string& host, unsigned short port, Callback callback);
    void onResolved(const boost::system::error_code& ec, ip::tcp::resolver::results_type results);

    template <typename Callback>
    void connect(ip::tcp::resolver::results_type results, Callback callback);
    void onConnected(const boost::system::error_code& ec, ip::tcp::resolver::iterator);

    template <typename Callback>
    void handshake(Callback callback);
    void onHandshaked(const boost::system::error_code& ec);

    template <typename Callback>
    void write(Callback callback);
    void onWritten(const boost::system::error_code& ec, std::size_t bytesTransferred);

    template <typename Callback>
    void read(Callback callback);
    void onRead(const boost::system::error_code& ec, std::size_t bytesTransferred);

private:
    ip::tcp::resolver m_resolver;
    std::unique_ptr<ssl::stream<ip::tcp::socket>> m_socket;
    HostInfo m_hostInfo;
    http::request<http::string_body> m_request;
    http::response_parser<http::string_body> m_response;
    boost::beast::flat_buffer m_buffer;
    boost::promise<HttpResponseResult> m_result;
    std::atomic<bool> m_resultSet = false;
};
