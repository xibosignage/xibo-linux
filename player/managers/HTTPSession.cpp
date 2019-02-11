#include "HTTPSession.hpp"

#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/rfc2818_verification.hpp>

const int DEFAULT_HTTP_VERSION = 11;

HTTPSession::HTTPSession(boost::asio::io_context& ioc) : m_resolver{ioc}
{
    boost::asio::ssl::context ctx{boost::asio::ssl::context::sslv23_client};
    ctx.set_default_verify_paths();
    ctx.set_verify_mode(boost::asio::ssl::verify_peer);

    m_socket = std::make_unique<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>(ioc, ctx);
    m_response.body_limit(std::numeric_limits<std::uint64_t>::max());
}

boost::future<HTTPResponseResult> HTTPSession::send(http::verb method, const Url& url, const std::string& body)
{
    m_request = createRequest(method, url.host, url.target, body);
    m_scheme = url.scheme;
    m_socket->set_verify_callback(ssl::rfc2818_verification(url.host));

    resolve(url.host, url.port, std::bind(&HTTPSession::onResolved, shared_from_this(), std::placeholders::_1, std::placeholders::_2));

    return m_result.get_future();
}

http::request<http::string_body> HTTPSession::createRequest(http::verb method, const std::string& host, const std::string& target, const std::string& body)
{
    http::request<http::string_body> request;

    request.method(method);
    request.target(target);
    request.version(DEFAULT_HTTP_VERSION);
    request.set(http::field::host, host);
    request.body() = body;
    request.prepare_payload();

    return request;
}

template<typename Callback>
void HTTPSession::resolve(const Url::Host& host, unsigned short port, Callback callback)
{    
    m_resolver.async_resolve(std::string{host}, std::to_string(port), ip::resolver_base::numeric_service, callback);
}

void HTTPSession::onResolved(const boost::system::error_code& ec, ip::tcp::resolver::results_type results)
{ 
    if(!ec)
    {
        connect(results, std::bind(&HTTPSession::onConnected, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        sessionFinished(ec);
    }
}

template<typename Callback>
void HTTPSession::connect(ip::tcp::resolver::results_type results, Callback callback)
{   
    asio::async_connect(m_socket->next_layer(), results.begin(), results.end(), callback);
}

void HTTPSession::onConnected(const boost::system::error_code& ec, ip::tcp::resolver::iterator)
{
    if(!ec)
    {
        if(m_scheme == Url::Scheme::HTTPS)
        {
            handshake(std::bind(&HTTPSession::onHandshaked, shared_from_this(), std::placeholders::_1));
        }
        else
        {
            write(std::bind(&HTTPSession::onWritten, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
        }
    }
    else
    {
        sessionFinished(ec);
    }
}

template<typename Callback>
void HTTPSession::handshake(Callback callback)
{
    m_socket->async_handshake(ssl::stream_base::client, callback);
}

void HTTPSession::onHandshaked(const boost::system::error_code& ec)
{
    if(!ec)
    {
        write(std::bind(&HTTPSession::onWritten, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        sessionFinished(ec);
    }
}

template<typename Callback>
void HTTPSession::write(Callback callback)
{
    if(m_scheme == Url::Scheme::HTTPS)
    {
        http::async_write(*m_socket, m_request, callback);
    }
    else
    {
        http::async_write(m_socket->next_layer(), m_request, callback);
    }
}

void HTTPSession::onWritten(const boost::system::error_code& ec, std::size_t /*bytes*/)
{
    if(!ec)
    {
        read(std::bind(&HTTPSession::onRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        sessionFinished(ec);
    }
}

template<typename Callback>
void HTTPSession::read(Callback callback)
{
    if(m_scheme == Url::Scheme::HTTPS)
    {
        http::async_read(*m_socket, m_buffer, m_response, callback);
    }
    else
    {
        http::async_read(m_socket->next_layer(), m_buffer, m_response, callback);
    }
}

void HTTPSession::onRead(const boost::system::error_code& ec, std::size_t /*bytes*/)
{
    sessionFinished(ec);
}

void HTTPSession::sessionFinished(const boost::system::error_code& ec)
{
    PlayerError error = ec ? PlayerError{PlayerError::Type::HTTP, ec.message()} : PlayerError{};

    setHttpResult(HTTPResponseResult{error, m_response.get().body()});
}

void HTTPSession::setHttpResult(const HTTPResponseResult& result)
{
    if(!m_resultSet)
    {
        m_resultSet = true;
        m_result.set_value(result);
    }
}

void HTTPSession::cancel()
{
    setHttpResult(HTTPResponseResult{PlayerError{PlayerError::Type::HTTP, "Operation Aborted"}, {}});
}
