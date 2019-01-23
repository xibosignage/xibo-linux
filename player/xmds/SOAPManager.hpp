#pragma once

#include "SOAP.hpp"

#include "utils/ResponseResult.hpp"
#include "utils/JoinableThread.hpp"
#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "managers/HTTPManager.hpp"

#include <boost/noncopyable.hpp>

const std::string XMDS_TARGET = "/xmds.php?v=5";

class SOAPManager : private boost::noncopyable
{
public:
    SOAPManager(const std::string& host) : m_host(host)
    {
    }

    template<typename Result, typename Request>
    boost::future<ResponseResult<Result>> sendRequest(const Request& soapRequest)
    {
        static_assert(std::is_copy_assignable_v<Request> && std::is_copy_constructible_v<Request>);

        std::string url = m_host + XMDS_TARGET;
        SOAP::RequestSerializer<Request> serializer{soapRequest};

        return Utils::httpManager().post(url, serializer.string()).then([this](boost::future<HTTPResponseResult> future){
            return onResponseReceived<Result>(future.get());
        });
    }

    template<typename Result>
    ResponseResult<Result> onResponseReceived(const HTTPResponseResult& httpResponse)
    {
        auto [httpError, httpResult] = httpResponse;
        if(httpError) return ResponseResult<Result>{httpError, {}};

        SOAP::ResponseParser<Result> parser(httpResult);
        return parser.get();
    }

private:
    std::string m_host;

};
