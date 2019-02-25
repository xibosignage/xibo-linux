#pragma once

#include "utils/JoinableThread.hpp"
#include "utils/ResponseResult.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/thread/future.hpp>
#include <boost/noncopyable.hpp>
#include <boost/beast/http/verb.hpp>

using HTTPResponseResult = ResponseResult<std::string>;
class HTTPSession;
class Uri;

class HTTPManager : private boost::noncopyable
{
public:
    HTTPManager();
    ~HTTPManager();

    void shutdown();
    boost::future<HTTPResponseResult> get(const std::string& url);
    boost::future<HTTPResponseResult> post(const std::string& url, const std::string& body);

private:
    boost::future<HTTPResponseResult> send(boost::beast::http::verb method, const Uri& uri, const std::string& body);

    boost::future<HTTPResponseResult> managerStoppedError();
    void cancelActiveSession();

private:
    boost::asio::io_context m_ioc;
    boost::asio::io_context::work m_work;
    std::vector<std::unique_ptr<JoinableThread>> m_workerThreads;
    std::vector<std::weak_ptr<HTTPSession>> m_activeSessions;
};
