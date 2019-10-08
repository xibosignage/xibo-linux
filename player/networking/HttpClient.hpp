#pragma once

#include "common/JoinableThread.hpp"
#include "common/types/Uri.hpp"
#include "networking/ResponseResult.hpp"

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
    void setProxyServer(const boost::optional<Uri>& uri);
    boost::future<HttpResponseResult> get(const Uri& uri);
    boost::future<HttpResponseResult> post(const Uri& uri, const std::string& body);

private:
    HttpClient();

    boost::future<HttpResponseResult> send(boost::beast::http::verb method, const Uri& uri, const std::string& body);

    boost::future<HttpResponseResult> managerStoppedError();
    void cancelActiveSession();

private:
    boost::asio::io_context ioc_;
    boost::asio::io_context::work work_;
    std::vector<std::unique_ptr<JoinableThread>> workerThreads_;
    std::vector<std::weak_ptr<HttpSession>> activeSessions_;
    boost::optional<Uri> proxy_;
};
