#pragma once

#include "common/crypto/CryptoUtils.hpp"
#include "common/types/Uri.hpp"
#include "common/constants.hpp"

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>

namespace http = boost::beast::http;

const std::string DefaultHttpTarget = "/";

class ProxyHttpRequest
{
public:
    ProxyHttpRequest(http::verb method,
                     const boost::optional<Uri::UserInfo>& proxyUserInfo,
                     const Uri& target,
                     const std::string& body) :
        method_(method),
        proxyUserInfo_(proxyUserInfo),
        target_(target),
        body_(std::move(body))
    {
    }

    http::request<http::string_body> get()
    {
        http::request<http::string_body> request;

        request.method(method_);
        request.target(target_.string());
        request.version(DefaultHttpVersion);
        request.set(http::field::host,
                    static_cast<std::string>(target_.authority().host()));  // TODO: check host with port
        if (proxyUserInfo_)
        {
            request.set(http::field::proxy_authorization,
                        "Basic " + CryptoUtils::toBase64(static_cast<std::string>(proxyUserInfo_.value())));
        }
        request.body() = std::move(body_);
        request.prepare_payload();

        return request;
    }

private:
    http::verb method_;
    boost::optional<Uri::UserInfo> proxyUserInfo_;
    Uri target_;
    std::string body_;
};
