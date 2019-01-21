#ifndef SOAPMANAGER_HPP
#define SOAPMANAGER_HPP

#include "SOAP.hpp"
#include "SOAPError.hpp"
#include "Session.hpp"

#include "utils/JoinableThread.hpp"
#include "utils/Logger.hpp"

#include <boost/noncopyable.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/asio/connect.hpp>
#include <future>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

class SOAPManager : private boost::noncopyable
{
public:
    SOAPManager(const std::string& host);
    ~SOAPManager();

    template<typename Result, typename Request>
    std::future<Result> sendRequest(const Request& soapRequest)
    {
        static_assert(std::is_copy_assignable_v<Request> && std::is_copy_constructible_v<Request>);

        auto session = std::make_shared<Session<Result, Request>>(m_ioc);
        session->soapRequest = soapRequest;

        return exec(session);
    }

private:
    template<typename Result, typename Request>
    std::future<Result> exec(std::shared_ptr<Session<Result, Request>> session)
    {
        auto resolve = std::bind(&SOAPManager::onResolve<Result, Request>, this, std::placeholders::_1, std::placeholders::_2, session);
        session->resolver.async_resolve(m_host, std::to_string(m_port), ip::resolver_base::numeric_service, resolve);

        return session->promise.get_future();
    }

    template<typename Result, typename Request>
    void sessionFinished(const SOAP::Error& err, std::shared_ptr<Session<Result, Request>> session)
    {
        Log::debug("Session finished with error [{}]: {}", err.faultCode(), err.faultMessage());
        session->promise.set_value({});
    }

    template<typename Result, typename Request>
    void onResolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results, std::shared_ptr<Session<Result, Request>> session)
    {
        if(!ec)
        {
            auto connect = std::bind(&SOAPManager::onConnect<Result, Request>, this, std::placeholders::_1, std::placeholders::_2, session);
            boost::asio::async_connect(session->socket, results.begin(), results.end(), connect);
        }
        else
        {
            sessionFinished(SOAP::Error{"HTTP", ec.message()}, session);
        }
    }

    template<typename Result, typename Request>
    void onConnect(const boost::system::error_code& ec, ip::tcp::resolver::iterator, std::shared_ptr<Session<Result, Request>> session)
    {
        if(!ec)
        {
            SOAP::RequestSerializer<Request> serializer{session->soapRequest};

            session->httpRequest.method(http::verb::post);
            session->httpRequest.target("/xmds.php?v=5");
            session->httpRequest.version(11);
            session->httpRequest.set(http::field::host, m_host);
            session->httpRequest.body() = serializer.string();
            session->httpRequest.prepare_payload();

            Log::trace("SOAP Request string: {}", serializer.string());

            auto write = std::bind(&SOAPManager::onWriteSoap<Result, Request>, this, std::placeholders::_1, std::placeholders::_2, session);
            boost::beast::http::async_write(session->socket, session->httpRequest, write);
        }
        else
        {
            sessionFinished(SOAP::Error{"HTTP", ec.message()}, session);
        }
    }

    template<typename Result, typename Request>
    void onWriteSoap(const boost::system::error_code& ec, std::size_t, std::shared_ptr<Session<Result, Request>> session)
    {
        if(!ec)
        {
            auto read = std::bind(&SOAPManager::onReadSoap<Result, Request>, this, std::placeholders::_1, std::placeholders::_2, session);
            boost::beast::http::async_read(session->socket, session->buffer, session->httpResponse, read);
        }
        else
        {
            sessionFinished(SOAP::Error{"HTTP", ec.message()}, session);
        }
    }

    template<typename Result, typename Request>
    void onReadSoap(const boost::system::error_code& ec, std::size_t, std::shared_ptr<Session<Result, Request>> session)
    {
        if(!ec)
        {
            Log::trace("SOAP Response string: {}", session->httpResponse.body());

            SOAP::ResponseParser<Result> parser(session->httpResponse.body());
            auto [error, response] = parser.get();
            if(error)
            {
                sessionFinished(error, session);
            }
            else
            {
                session->promise.set_value(response);
            }
        }
        else
        {
            sessionFinished(SOAP::Error{"HTTP", ec.message()}, session);
        }
    }


private:
    asio::io_context m_ioc;
    asio::io_context::work m_work;
    std::vector<std::unique_ptr<JoinableThread>> m_workerThreads;
    std::string m_host;
    int m_port;
};

#endif // SOAPMANAGER_HPP
