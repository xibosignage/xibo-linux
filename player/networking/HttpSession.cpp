#include "HttpSession.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/rfc2818_verification.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>

namespace ph = std::placeholders;

HttpSession::HttpSession(boost::asio::io_context& ioc) : resolver_{ioc}
{
    ssl::context ctx{ssl::context::sslv23_client};
    ctx.set_default_verify_paths();
    ctx.set_verify_mode(ssl::verify_peer);

    socket_ = std::make_unique<ssl::stream<ip::tcp::socket>>(ioc, ctx);
    response_.body_limit(std::numeric_limits<std::uint64_t>::max());
}

boost::future<HttpResponseResult> HttpSession::send(const HostInfo& info,
                                                    const http::request<http::string_body>& request)
{
    hostInfo_ = info;
    request_ = request;

    socket_->set_verify_callback(ssl::rfc2818_verification(hostInfo_.host));

    if (!SSL_set_tlsext_host_name(socket_->native_handle(), hostInfo_.host.c_str()))
    {
        boost::beast::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        sessionFinished(ec);
    }

    resolve(hostInfo_.host, hostInfo_.port, std::bind(&HttpSession::onResolved, shared_from_this(), ph::_1, ph::_2));

    return result_.get_future();
}

template <typename Callback>
void HttpSession::resolve(const std::string& host, unsigned short port, Callback callback)
{
    resolver_.async_resolve(host, std::to_string(port), ip::resolver_base::numeric_service, callback);
}

void HttpSession::onResolved(const boost::system::error_code& ec, ip::tcp::resolver::results_type results)
{
    if (!ec)
    {
        connect(results, std::bind(&HttpSession::onConnected, shared_from_this(), ph::_1, ph::_2));
    }
    else
    {
        sessionFinished(ec);
    }
}

template <typename Callback>
void HttpSession::connect(ip::tcp::resolver::results_type results, Callback callback)
{
    boost::asio::async_connect(socket_->next_layer(), results.begin(), results.end(), callback);
}

void HttpSession::onConnected(const boost::system::error_code& ec, ip::tcp::resolver::iterator)
{
    if (!ec)
    {
        if (hostInfo_.useSsl)
        {
            handshake(std::bind(&HttpSession::onHandshaked, shared_from_this(), ph::_1));
        }
        else
        {
            write(std::bind(&HttpSession::onWritten, shared_from_this(), ph::_1, ph::_2));
        }
    }
    else
    {
        sessionFinished(ec);
    }
}

template <typename Callback>
void HttpSession::handshake(Callback callback)
{
    socket_->async_handshake(ssl::stream_base::client, callback);
}

void HttpSession::onHandshaked(const boost::system::error_code& ec)
{
    if (!ec)
    {
        write(std::bind(&HttpSession::onWritten, shared_from_this(), ph::_1, ph::_2));
    }
    else
    {
        sessionFinished(ec);
    }
}

template <typename Callback>
void HttpSession::write(Callback callback)
{
    if (hostInfo_.useSsl)
    {
        http::async_write(*socket_, request_, callback);
    }
    else
    {
        http::async_write(socket_->next_layer(), request_, callback);
    }
}

void HttpSession::onWritten(const boost::system::error_code& ec, std::size_t /*bytes*/)
{
    if (!ec)
    {
        read(std::bind(&HttpSession::onRead, shared_from_this(), ph::_1, ph::_2));
    }
    else
    {
        sessionFinished(ec);
    }
}

template <typename Callback>
void HttpSession::read(Callback callback)
{
    if (hostInfo_.useSsl)
    {
        http::async_read(*socket_, buffer_, response_, callback);
    }
    else
    {
        http::async_read(socket_->next_layer(), buffer_, response_, callback);
    }
}

void HttpSession::onRead(const boost::system::error_code& ec, std::size_t /*bytes*/)
{
    sessionFinished(ec);
}

void HttpSession::sessionFinished(const boost::system::error_code& ec)
{
    if (!ec)
    {
        setHttpResult(HttpResponseResult{PlayerError{}, response_.get().body()});
    }
    else
    {
        PlayerError error{PlayerError::Type::HTTP, ec.message()};
        setHttpResult(HttpResponseResult{error, {}});
    }
}

void HttpSession::cancel()
{
    setHttpResult(HttpResponseResult{PlayerError{PlayerError::Type::HTTP, "Operation Aborted"}, {}});
}

void HttpSession::setHttpResult(const HttpResponseResult& result)
{
    if (!resultSet_)
    {
        resultSet_ = true;
        result_.set_value(result);
    }
}
