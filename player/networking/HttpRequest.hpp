#pragma once

#include "HostInfo.hpp"
#include "common/Utils.hpp"
#include "common/uri/Uri.hpp"
#include "constants.hpp"

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

    HostInfo hostInfo()
    {
        return HostInfo{uri_.host(), uri_.port(), uri_.scheme() == Uri::Scheme::HTTPS};
    }

    http::request<http::string_body> get()
    {
        http::request<http::string_body> request;

        request.method(method_);
        request.target(uri_.path());
        request.version(DefaultHttpVersion);
        request.set(http::field::host, uri_.host());
        if (auto credentials = uri_.credentials())
        {
            request.set(http::field::authorization, "Basic " + Utils::toBase64(credentials.value()));
        }
        request.body() = std::move(body_);
        request.prepare_payload();

        return request;
    }

private:
    http::verb method_;
    Uri uri_;
    std::string body_;
};
