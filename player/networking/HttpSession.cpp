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

boost::future<HttpResponseResult> HttpSession::send(const Uri& uri, const http::request<http::string_body>& request)
{
    useSsl_ = uri.scheme() == Uri::HttpsScheme;
    request_ = request;

    auto host = uri.authority().host();
    auto hostString = static_cast<std::string>(host);

    socket_->set_verify_callback(ssl::rfc2818_verification(hostString));
    if (!SSL_set_tlsext_host_name(socket_->native_handle(), hostString.data()))
    {
        boost::beast::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        sessionFinished(ec);
    }

    resolve(uri.authority().host(),
            uri.authority().port(),
            std::bind(&HttpSession::onResolved, shared_from_this(), ph::_1, ph::_2));

    return result_.get_future();
}

template <typename Callback>
void HttpSession::resolve(const Uri::Host& host, const Uri::Port& port, Callback callback)
{
    resolver_.async_resolve(
        static_cast<std::string>(host), port.string(), ip::resolver_base::numeric_service, callback);
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
        if (useSsl_)
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
    if (useSsl_)
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
    if (useSsl_)
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
        auto&& message = response_.get();
        if (message.result() == http::status::ok)
        {
            setHttpResult(HttpResponseResult{PlayerError{}, response_.get().body()});
        }
        else
        {
            std::string errorMessage = std::to_string(message.result_int()) + " " + std::string{message.reason()};
            PlayerError error{"HTTP", errorMessage};
            setHttpResult(HttpResponseResult{error, {}});
        }
    }
    else
    {
        PlayerError error{"HTTP", ec.message()};
        setHttpResult(HttpResponseResult{error, {}});
    }
}

void HttpSession::cancel()
{
    setHttpResult(HttpResponseResult{PlayerError{"HTTP", "Operation Aborted"}, {}});
}

void HttpSession::setHttpResult(const HttpResponseResult& result)
{
    if (!resultSet_)
    {
        resultSet_ = true;
        result_.set_value(result);
    }
}
