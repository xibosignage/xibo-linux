#include "HttpSession.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/rfc2818_verification.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>

namespace ph = std::placeholders;

HttpSession::HttpSession(boost::asio::io_context& ioc) : m_resolver{ioc}
{
    ssl::context ctx{ssl::context::sslv23_client};
    ctx.set_default_verify_paths();
    ctx.set_verify_mode(ssl::verify_peer);

    m_socket = std::make_unique<ssl::stream<ip::tcp::socket>>(ioc, ctx);
    m_response.body_limit(std::numeric_limits<std::uint64_t>::max());
}

boost::future<HttpResponseResult> HttpSession::send(const HostInfo& info,
                                                    const http::request<http::string_body>& request)
{
    m_hostInfo = info;
    m_request = request;

    m_socket->set_verify_callback(ssl::rfc2818_verification(m_hostInfo.host));

    if (!SSL_set_tlsext_host_name(m_socket->native_handle(), m_hostInfo.host.c_str()))
    {
        boost::beast::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        sessionFinished(ec);
    }

    resolve(m_hostInfo.host, m_hostInfo.port, std::bind(&HttpSession::onResolved, shared_from_this(), ph::_1, ph::_2));

    return m_result.get_future();
}

template <typename Callback>
void HttpSession::resolve(const std::string& host, unsigned short port, Callback callback)
{
    m_resolver.async_resolve(host, std::to_string(port), ip::resolver_base::numeric_service, callback);
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
    boost::asio::async_connect(m_socket->next_layer(), results.begin(), results.end(), callback);
}

void HttpSession::onConnected(const boost::system::error_code& ec, ip::tcp::resolver::iterator)
{
    if (!ec)
    {
        if (m_hostInfo.useSsl)
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
    m_socket->async_handshake(ssl::stream_base::client, callback);
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
    if (m_hostInfo.useSsl)
    {
        http::async_write(*m_socket, m_request, callback);
    }
    else
    {
        http::async_write(m_socket->next_layer(), m_request, callback);
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
    if (m_hostInfo.useSsl)
    {
        http::async_read(*m_socket, m_buffer, m_response, callback);
    }
    else
    {
        http::async_read(m_socket->next_layer(), m_buffer, m_response, callback);
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
        setHttpResult(HttpResponseResult{PlayerError{}, m_response.get().body()});
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
    if (!m_resultSet)
    {
        m_resultSet = true;
        m_result.set_value(result);
    }
}
