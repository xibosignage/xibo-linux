#pragma once

#include "HostInfo.hpp"
#include "ProxyInfo.hpp"
#include "common/Utils.hpp"
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
        proxyUri_(Uri::fromString(info.host + DefaultHttpTarget)),
        username_(info.username),
        password_(info.password),
        uri_(uri),
        body_(std::move(body))
    {
    }

    HostInfo hostInfo()
    {
        return HostInfo{proxyUri_.host(), proxyUri_.port(), proxyUri_.scheme() == Uri::Scheme::HTTPS};
    }

    http::request<http::string_body> get()
    {
        http::request<http::string_body> request;

        request.method(method_);
        request.target(uri_.string());
        request.version(DefaultHttpVersion);
        request.set(http::field::host, uri_.host());
        if (auto credentials = getCredentials())
        {
            request.set(http::field::proxy_authorization, "Basic " + Utils::toBase64(credentials.value()));
        }
        request.body() = std::move(body_);
        request.prepare_payload();

        return request;
    }

    boost::optional<std::string> getCredentials()
    {
        if (!username_.empty() && !password_.empty())
        {
            return username_ + ":" + password_;
        }

        return {};
    }

private:
    http::verb method_;
    Uri proxyUri_;
    std::string username_;
    std::string password_;
    Uri uri_;
    std::string body_;
};
