#pragma once

#include "utils/JoinableThread.hpp"
#include "utils/ResponseResult.hpp"
#include "utils/uri/Uri.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/thread/future.hpp>
#include <boost/noncopyable.hpp>
#include <boost/beast/http/verb.hpp>

using HttpResponseResult = ResponseResult<std::string>;
class HttpSession;
class Uri;

class HttpManager : private boost::noncopyable
{
public:
    HttpManager();
    ~HttpManager();

    void shutdown();
    boost::future<HttpResponseResult> get(const Uri& uri);
    boost::future<HttpResponseResult> post(const Uri& uri, const std::string& body);

private:
    boost::future<HttpResponseResult> send(boost::beast::http::verb method, const Uri& uri, const std::string& body);

    boost::future<HttpResponseResult> managerStoppedError();
    void cancelActiveSession();

private:
    boost::asio::io_context m_ioc;
    boost::asio::io_context::work m_work;
    std::vector<std::unique_ptr<JoinableThread>> m_workerThreads;
    std::vector<std::weak_ptr<HttpSession>> m_activeSessions;

};
