#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>

#include <boost/thread/future.hpp>

#include "common/types/Uri.hpp"
#include "networking/ResponseResult.hpp"

namespace http = boost::beast::http;
namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;

using HttpResponseResult = ResponseResult<std::string>;

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:
    HttpSession(boost::asio::io_context& ioc);

    boost::future<HttpResponseResult> send(const Uri& uri, const http::request<http::string_body>& request);
    void cancel();

private:
    void sessionFinished(const boost::system::error_code& ec);
    void setHttpResult(const HttpResponseResult& result);

    template <typename Callback>
    void resolve(const Uri::Host& host, const Uri::Port& port, Callback callback);
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
    ip::tcp::resolver resolver_;
    bool useSsl_ = false;
    std::unique_ptr<ssl::stream<ip::tcp::socket>> socket_;
    http::request<http::string_body> request_;
    http::response_parser<http::string_body> response_;
    boost::beast::flat_buffer buffer_;
    boost::promise<HttpResponseResult> result_;
    std::atomic<bool> resultSet_ = false;
};
