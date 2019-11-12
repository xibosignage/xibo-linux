#pragma once

#include "networking/HttpClient.hpp"
#include "networking/ResponseResult.hpp"
#include "cms/xmds/Soap.hpp"

namespace SoapRequestHelper
{
    template <typename Result>
    ResponseResult<Result> onResponseReceived(const HttpResponseResult& httpResponse)
    {
        auto [httpError, httpResult] = httpResponse;
        if (httpError) return ResponseResult<Result>{httpError, {}};

        Soap::ResponseParser<Result> parser(httpResult);
        return parser.get();
    }

    template <typename Result, typename Request>
    boost::future<ResponseResult<Result>> sendRequest(const Uri& uri, const Request& soapRequest)
    {
        static_assert(std::is_copy_assignable_v<Request> && std::is_copy_constructible_v<Request>);

        Soap::RequestSerializer<Request> serializer{soapRequest};

        return HttpClient::instance().post(uri, serializer.string()).then([](boost::future<HttpResponseResult> future) {
            return onResponseReceived<Result>(future.get());
        });
    }
}
