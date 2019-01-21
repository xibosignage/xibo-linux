#pragma once

#include "SOAP.hpp"
#include "SOAPError.hpp"

#include "utils/JoinableThread.hpp"
#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "managers/HTTPManager.hpp"

#include <boost/noncopyable.hpp>
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

        auto result = std::make_shared<std::promise<Result>>();
        std::string url = m_host + "/xmds.php?v=5";
        SOAP::RequestSerializer<Request> serializer{soapRequest};

        Utils::httpManager().post(url, serializer.string(), std::bind(&SOAPManager::onResponseReceived<Result>, this, std::placeholders::_1, result));

        return result->get_future();
    }

    template<typename Result>
    void onResponseReceived(const ResponseResult& httpResponse, std::shared_ptr<std::promise<Result>> result)
    {
        if(!httpResponse.error)
        {
            SOAP::ResponseParser<Result> parser(httpResponse.result);
            auto [error, soapResponse] = parser.get();
            if(!error)
            {
                result->set_value(soapResponse);
            }
            else
            {
                reportRequestError(error, result);
            }
        }
        else
        {
            reportRequestError(SOAP::Error{"HTTP", httpResponse.error.message()}, result);
        }
    }

    template<typename Result>
    void reportRequestError(const SOAP::Error& error, std::shared_ptr<std::promise<Result>> result)
    {
        Log::debug("Session finished with error [{}]: {}", error.faultCode(), error.faultMessage());
        result->set_value({});
    }

private:
    asio::io_context m_ioc;
    asio::io_context::work m_work;
    std::vector<std::unique_ptr<JoinableThread>> m_workerThreads;
    std::string m_host;
};
