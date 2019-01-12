#ifndef SOAPMANAGER_HPP
#define SOAPMANAGER_HPP

#include "SessionExecutor.hpp"

#include <boost/noncopyable.hpp>
#include <thread>
#include <future>

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

        auto sessionExecutor = std::make_shared<SessionExecutor<Result, Request>>(m_host, m_port, session);
        return sessionExecutor->exec();
    }

private:
    asio::io_context m_ioc;
    asio::io_context::work m_work;
    std::vector<std::unique_ptr<std::thread>> m_workThreads;
    std::string m_host;
    int m_port;
};

#endif // SOAPMANAGER_HPP
