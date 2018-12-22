#ifndef SOAPMANAGER_HPP
#define SOAPMANAGER_HPP

#include "SessionExecutor.hpp"

#include <boost/noncopyable.hpp>
#include <thread>

class SOAPManager : private boost::noncopyable
{
public:
    SOAPManager(const std::string& host);
    ~SOAPManager();

    template<typename Response, typename Request>
    void sendRequest(const Request& soapRequest, const typename Session<Response, Request>::callback& responseCallback)
    {
        static_assert(std::is_copy_assignable_v<Request> && std::is_copy_constructible_v<Request>);

        auto session = std::make_shared<Session<Response, Request>>(m_ioc);
        session->responseCallback = responseCallback;
        session->soapRequest = soapRequest;

        auto e = std::make_shared<SessionExecutor<Response, Request>>(m_host, m_port, session);
        e->exec();
    }

private:
    asio::io_context m_ioc;
    asio::io_context::work m_work;
    std::unique_ptr<std::thread> m_workThread;
    std::string m_host;
    int m_port;
};

#endif // SOAPMANAGER_HPP
