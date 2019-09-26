#pragma once

#include "common/JoinableThread.hpp"
#include "common/uri/Uri.hpp"

#include "ProxyInfo.hpp"
#include "ResponseResult.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/future.hpp>

using HttpResponseResult = ResponseResult<std::string>;
class HttpSession;
class Uri;

class HttpClient : private boost::noncopyable
{
public:
    ~HttpClient();

    static HttpClient& instance();

    void shutdown();
    void setProxyServer(const std::string& host, const std::string& username, const std::string& password);
    boost::future<HttpResponseResult> get(const Uri& uri);
    boost::future<HttpResponseResult> post(const Uri& uri, const std::string& body);

private:
    HttpClient();

    boost::future<HttpResponseResult> send(boost::beast::http::verb method, const Uri& uri, const std::string& body);

    boost::future<HttpResponseResult> managerStoppedError();
    void cancelActiveSession();

private:
    boost::asio::io_context m_ioc;
    boost::asio::io_context::work m_work;
    std::vector<std::unique_ptr<JoinableThread>> m_workerThreads;
    std::vector<std::weak_ptr<HttpSession>> m_activeSessions;
    boost::optional<ProxyInfo> m_proxyInfo;
};
