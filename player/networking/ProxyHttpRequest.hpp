#pragma once

#include "ProxyInfo.hpp"
#include "HostInfo.hpp"
#include "constants.hpp"
#include "common/uri/Uri.hpp"

#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/core/detail/base64.hpp>

namespace http = boost::beast::http;

class ProxyHttpRequest
{
public:
    ProxyHttpRequest(http::verb method, const ProxyInfo& info, const Uri& uri, const std::string& body) :
        m_method(method),
        m_proxyUri(info.host + DEFAULT_HTTP_TARGET),
        m_username(info.username),
        m_password(info.password),
        m_uri(uri),
        m_body(std::move(body))
    {
    }

    HostInfo hostInfo()
    {
        return HostInfo{m_proxyUri.host(), m_proxyUri.port(), m_proxyUri.scheme() == Uri::Scheme::HTTPS};
    }

    http::request<http::string_body> get()
    {
        http::request<http::string_body> request;

        request.method(m_method);
        request.target(m_uri.string());
        request.version(DEFAULT_HTTP_VERSION);
        request.set(http::field::host, m_uri.host());
        if(auto credentials = getCredentials())
        {
            request.set(http::field::proxy_authorization, "Basic " + boost::beast::detail::base64_encode(credentials.value()));
        }
        request.body() = std::move(m_body);
        request.prepare_payload();

        return request;
    }

    boost::optional<std::string> getCredentials()
    {
        if(!m_username.empty() && !m_password.empty())
        {
            return m_username + ":" + m_password;
        }

        return {};
    }

private:
    http::verb m_method;
    Uri m_proxyUri;
    std::string m_username;
    std::string m_password;
    Uri m_uri;
    std::string m_body;


};
