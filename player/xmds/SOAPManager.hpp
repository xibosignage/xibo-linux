#ifndef SOAPMANAGER_HPP
#define SOAPMANAGER_HPP

#include <functional>
#include <spdlog/spdlog.h>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include "soap.hpp"
#include "constants.hpp"

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

template<typename request>
struct Session
{
    using callback = std::function<void(typename soap::request_traits<request>::response_t)>;
    Session(asio::io_context& ioc) : socket(ioc), resolver(ioc) { }
    ip::tcp::socket socket;
    ip::tcp::resolver resolver;
    http::request<http::string_body> http_request;
    beast::flat_buffer buffer;
    http::response<http::string_body> http_response;
    request soap_request;
    callback response_callback; // NOTE: check if it should be move/copyable
};

class SOAPManager
{
public:
    SOAPManager(const std::string& host);
    ~SOAPManager();
    SOAPManager(const SOAPManager&) = delete;
    SOAPManager& operator=(const SOAPManager&) = delete;

    template<typename request>
    void send_request(const request& soap_request, typename Session<request>::callback&& response_callback)
    {
        static_assert(std::is_copy_assignable_v<request> && std::is_copy_constructible_v<request>);

        auto session = std::make_shared<Session<request>>(m_ioc);
        session->response_callback = response_callback;
        session->soap_request = soap_request;

        auto resolve = std::bind(&SOAPManager::on_resolve<request>, this, std::placeholders::_1, std::placeholders::_2, session);
        session->resolver.async_resolve(m_host, std::to_string(m_port), ip::resolver_base::numeric_service, resolve);
    }

private:
    template<typename request>
    void on_read_soap(const boost::system::error_code& ec, std::size_t, std::shared_ptr<Session<request>> session)
    {
        if(!ec)
        {
            if(session->http_response.result() == http::status::ok)
            {
                using response = typename soap::request_traits<request>::response_t;
                auto result = soap::create_response<response>(session->http_response.body());
                session->response_callback(result);
            }
            else
            {
                m_logger->error("Receive SOAP request with HTTP error: {}", session->http_response.result_int());
            }
        }
        else
        {
            m_logger->error("Receive SOAP request with error: {}", ec.message());
        }
    }

    template<typename request>
    void on_write_soap(const boost::system::error_code& ec, std::size_t, std::shared_ptr<Session<request>> session)
    {
        if(!ec)
        {
            auto read = std::bind(&SOAPManager::on_read_soap<request>, this, std::placeholders::_1, std::placeholders::_2, session);
            http::async_read(session->socket, session->buffer, session->http_response, read);
        }
        else
        {
            m_logger->error("Send SOAP request with error: {}", ec.message());
        }
    }

    template<typename request>
    void on_connect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, std::shared_ptr<Session<request>> session)
    {
        if(!ec)
        {
            session->http_request.method(http::verb::post);
            session->http_request.target("/xmds.php?v=5");
            session->http_request.version(11);
            session->http_request.set(http::field::host, m_host);
            session->http_request.body() = soap::request_string(session->soap_request);
            session->http_request.prepare_payload();

            m_logger->trace("SOAP Request string: {}", soap::request_string(session->soap_request));

            auto write = std::bind(&SOAPManager::on_write_soap<request>, this, std::placeholders::_1, std::placeholders::_2, session);
            http::async_write(session->socket, session->http_request, write);
        }
        else
        {
            m_logger->error("SOAP Connected to host with error: {}", ec.message());
        }
    }

    template<typename request>
    void on_resolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, std::shared_ptr<Session<request>> session)
    {
        if(!ec)
        {
            auto connect = std::bind(&SOAPManager::on_connect<request>, this, std::placeholders::_1, std::placeholders::_2, session);
            asio::async_connect(session->socket, results.begin(), results.end(), connect);
        }
        else
        {
            m_logger->error("SOAP Resolved host with error: {}", ec.message());
        }
    }

private:
    asio::io_context m_ioc;
    asio::io_context::work m_work;
    std::unique_ptr<std::thread> m_work_thread;
    std::string m_host;
    std::shared_ptr<spdlog::logger> m_logger;
    int m_port;
};

#endif // SOAPMANAGER_HPP
