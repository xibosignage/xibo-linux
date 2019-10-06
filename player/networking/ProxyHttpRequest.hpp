#pragma once

#include "networking/ProxyInfo.hpp"

#include "common/crypto/CryptoUtils.hpp"
#include "common/types/Uri.hpp"
#include "constants.hpp"

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>

namespace http = boost::beast::http;

const std::string DefaultHttpTarget = "/";

class ProxyHttpRequest
{
public:
    ProxyHttpRequest(http::verb method, const ProxyInfo& info, const Uri& uri, const std::string& body) :
        method_(method),
        username_(info.username),
        password_(info.password),
        uri_(uri),
        body_(std::move(body))
    {
    }

    http::request<http::string_body> get()
    {
        http::request<http::string_body> request;

        request.method(method_);
        request.target(uri_.string());
        request.version(DefaultHttpVersion);
        request.set(http::field::host, static_cast<std::string>(uri_.authority().host()));
        if (auto info = uri_.authority().optionalUserInfo())
        {
            request.set(http::field::proxy_authorization,
                        "Basic " + CryptoUtils::toBase64(static_cast<std::string>(info.value())));
        }
        request.body() = std::move(body_);
        request.prepare_payload();

        return request;
    }

private:
    http::verb method_;
    std::string username_;
    std::string password_;
    Uri uri_;
    std::string body_;
};
