#pragma once

#include "networking/HttpClient.hpp"
#include "networking/ResponseResult.hpp"

namespace Soap
{
    template <typename Result>
    ResponseResult<Result> onResponseReceived(const HttpResponseResult& httpResponse)
    {
        auto [httpError, httpResult] = httpResponse;
        if (httpError) return ResponseResult<Result>{httpError, {}};

        try
        {
            return {{}, Result::fromXmlString(httpResult)};
        }
        catch (PlayerRuntimeError& e)
        {
            return {e.error(), {}};
        }
        catch (...)
        {
            return {PlayerError{"XMDS", "Unknown error"}, {}};
        }
    }

    template <typename Result, typename Request>
    FutureResponseResult<Result> sendRequest(const Uri& uri, const Request& soapRequest)
    {
        static_assert(std::is_copy_assignable_v<Request> && std::is_copy_constructible_v<Request>);

        return HttpClient::instance()
            .post(uri, soapRequest.string())
            .then([](boost::future<HttpResponseResult> future) { return onResponseReceived<Result>(future.get()); });
    }
}
