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
    http::request<http::string_body> httpRequest;
    beast::flat_buffer buffer;
    http::response<http::string_body> httpResponse;
    request soapRequest;
    callback responseCallback; // NOTE: check if it should be move/copyable
};

class SOAPManager
{
public:
    SOAPManager(const std::string& host);
    ~SOAPManager();
    SOAPManager(const SOAPManager&) = delete;
    SOAPManager& operator=(const SOAPManager&) = delete;

    template<typename request>
    void sendRequest(const request& soapRequest, typename Session<request>::callback&& responseCallback)
    {
        static_assert(std::is_copy_assignable_v<request> && std::is_copy_constructible_v<request>);

        auto session = std::make_shared<Session<request>>(m_ioc);
        session->responseCallback = responseCallback;
        session->soapRequest = soapRequest;

        auto resolve = std::bind(&SOAPManager::onResolve<request>, this, std::placeholders::_1, std::placeholders::_2, session);
        session->resolver.async_resolve(m_host, std::to_string(m_port), ip::resolver_base::numeric_service, resolve);
    }

private:
    template<typename request>
    void onReadSoap(const boost::system::error_code& ec, std::size_t, std::shared_ptr<Session<request>> session)
    {
        if(!ec)
        {
            if(session->httpResponse.result() == http::status::ok)
            {
                using response = typename soap::request_traits<request>::response_t;
                auto result = soap::createResponse<response>(session->httpResponse.body());
                session->responseCallback(result);
            }
            else
            {
                m_logger->error("Receive SOAP request with HTTP error: {}", session->httpResponse.result_int());
            }
        }
        else
        {
            m_logger->error("Receive SOAP request with error: {}", ec.message());
        }
    }

    template<typename request>
    void onWriteSoap(const boost::system::error_code& ec, std::size_t, std::shared_ptr<Session<request>> session)
    {
        if(!ec)
        {
            auto read = std::bind(&SOAPManager::onReadSoap<request>, this, std::placeholders::_1, std::placeholders::_2, session);
            http::async_read(session->socket, session->buffer, session->httpResponse, read);
        }
        else
        {
            m_logger->error("Send SOAP request with error: {}", ec.message());
        }
    }

    template<typename request>
    void onConnect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, std::shared_ptr<Session<request>> session)
    {
        if(!ec)
        {
            session->httpRequest.method(http::verb::post);
            session->httpRequest.target("/xmds.php?v=5");
            session->httpRequest.version(11);
            session->httpRequest.set(http::field::host, m_host);
            session->httpRequest.body() = soap::requestString(session->soapRequest);
            session->httpRequest.prepare_payload();

            m_logger->trace("SOAP Request string: {}", soap::requestString(session->soapRequest));

            auto write = std::bind(&SOAPManager::onWriteSoap<request>, this, std::placeholders::_1, std::placeholders::_2, session);
            http::async_write(session->socket, session->httpRequest, write);
        }
        else
        {
            m_logger->error("SOAP Connected to host with error: {}", ec.message());
        }
    }

    template<typename request>
    void onResolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, std::shared_ptr<Session<request>> session)
    {
        if(!ec)
        {
            auto connect = std::bind(&SOAPManager::onConnect<request>, this, std::placeholders::_1, std::placeholders::_2, session);
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
    std::unique_ptr<std::thread> m_workThread;
    std::string m_host;
    std::shared_ptr<spdlog::logger> m_logger;
    int m_port;
};

#endif // SOAPMANAGER_HPP
