#pragma once

#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/asio/connect.hpp>
#include <future>

#include "utils/Logger.hpp"
#include "SOAP.hpp"
#include "Session.hpp"

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

template<typename Result, typename Request>
class SessionExecutor : public std::enable_shared_from_this<SessionExecutor<Result, Request>>
{
public:
    SessionExecutor(const std::string& host, int port, std::shared_ptr<Session<Result, Request>> session) :
        m_host(host), m_port(port), m_session(session)
    {
    }

    std::future<Result> exec()
    {
        auto resolve = std::bind(&SessionExecutor::onResolve, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2);
        m_session->resolver.async_resolve(m_host, std::to_string(m_port), ip::resolver_base::numeric_service, resolve);

        return m_promise.get_future();
    }

private:
    void sessionFinished(const SOAP::Error& err)
    {
        Log::debug("Session finished with error [{}]: {}", err.faultCode(), err.faultMessage());
        m_promise.set_value({});
    }

    void onResolve(const boost::system::error_code& ec, ip::tcp::resolver::results_type results)
    {
        if(!ec)
        {
            auto connect = std::bind(&SessionExecutor::onConnect, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2);
            boost::asio::async_connect(m_session->socket, results.begin(), results.end(), connect);
        }
        else
        {
            sessionFinished(SOAP::Error{"HTTP", ec.message()});
        }
    }

    void onConnect(const boost::system::error_code& ec, ip::tcp::resolver::iterator)
    {
        if(!ec)
        {
            SOAP::RequestSerializer<Request> serializer{m_session->soapRequest};

            m_session->httpRequest.method(http::verb::post);
            m_session->httpRequest.target("/xmds.php?v=5");
            m_session->httpRequest.version(11);
            m_session->httpRequest.set(http::field::host, m_host);
            m_session->httpRequest.body() = serializer.string();
            m_session->httpRequest.prepare_payload();

            Log::trace("SOAP Request string: {}", serializer.string());

            auto write = std::bind(&SessionExecutor::onWriteSoap, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2);
            boost::beast::http::async_write(m_session->socket, m_session->httpRequest, write);
        }
        else
        {
            sessionFinished(SOAP::Error{"HTTP", ec.message()});
        }
    }

    void onWriteSoap(const boost::system::error_code& ec, std::size_t)
    {
        if(!ec)
        {
            auto read = std::bind(&SessionExecutor::onReadSoap, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2);
            boost::beast::http::async_read(m_session->socket, m_session->buffer, m_session->httpResponse, read);
        }
        else
        {
            sessionFinished(SOAP::Error{"HTTP", ec.message()});
        }
    }

    void onReadSoap(const boost::system::error_code& ec, std::size_t)
    {
        if(!ec)
        {
            Log::trace("SOAP Response string: {}", m_session->httpResponse.body());

            SOAP::ResponseParser<Result> parser(m_session->httpResponse.body());
            auto [error, response] = parser.get();
            if(error)
            {
                sessionFinished(error);
            }
            else
            {
                m_promise.set_value(response);
            }
        }
        else
        {
            sessionFinished(SOAP::Error{"HTTP", ec.message()});
        }
    }

private:
    std::string m_host;
    int m_port;
    std::promise<Result> m_promise;
    std::shared_ptr<Session<Result, Request>> m_session;

};
