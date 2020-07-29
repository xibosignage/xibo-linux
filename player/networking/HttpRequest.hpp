#pragma once

#include "common/constants.hpp"
#include "common/crypto/CryptoUtils.hpp"
#include "common/types/Uri.hpp"

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>

namespace http = boost::beast::http;

class HttpRequest
{
public:
    HttpRequest(http::verb method, const Uri& uri, const std::string& body) :
        method_(method),
        uri_(uri),
        body_(std::move(body))
    {
    }

    http::request<http::string_body> get()
    {
        http::request<http::string_body> request;

        request.method(method_);
        request.target(uri_.path());
        request.version(DefaultHttpVersion);
        request.set(http::field::host, hostWithPort(uri_.authority()));
        if (auto userinfo = uri_.authority().optionalUserInfo())
        {
            request.set(http::field::authorization,
                        "Basic " + CryptoUtils::toBase64(static_cast<std::string>(userinfo.value())));
        }
        request.body() = std::move(body_);
        request.prepare_payload();

        return request;
    }

private:
    std::string hostWithPort(const Uri::Authority& authority)
    {
        auto&& host = authority.host();

        if (auto port = authority.optionalPort())
        {
            return static_cast<std::string>(host) + ":" + port->string();
        }

        return static_cast<std::string>(host);
    }

private:
    http::verb method_;
    Uri uri_;
    std::string body_;
};
