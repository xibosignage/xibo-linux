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
        m_method(method),
        m_uri(uri),
        m_body(std::move(body))
    {
    }

    HostInfo hostInfo()
    {
        return HostInfo{m_uri.host(), m_uri.port(), m_uri.scheme() == Uri::Scheme::HTTPS};
    }

    http::request<http::string_body> get()
    {
        http::request<http::string_body> request;

        request.method(m_method);
        request.target(m_uri.path());
        request.version(DefaultHttpVersion);
        request.set(http::field::host, m_uri.host());
        if (auto credentials = m_uri.credentials())
        {
            request.set(http::field::authorization, "Basic " + Utils::toBase64(credentials.value()));
        }
        request.body() = std::move(m_body);
        request.prepare_payload();

        return request;
    }

private:
    http::verb m_method;
    Uri m_uri;
    std::string m_body;
};
