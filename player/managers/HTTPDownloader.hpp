#pragma once

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <future>
#include "utils/JoinableThread.hpp"

struct DownloadedFile
{
    boost::system::error_code downloadError;
    std::string name;
};

using DownloadFinished = std::function<void()>;
using FileDownloadCallback = std::function<void(const DownloadedFile&)>;

struct DownloadSession;
using DownloadSessionPtr = std::shared_ptr<DownloadSession>;

class HTTPDownloader
{
public:
    HTTPDownloader();
    ~HTTPDownloader();
    HTTPDownloader(const HTTPDownloader&) = delete;
    HTTPDownloader& operator=(const HTTPDownloader&) = delete;

    std::future<void> download(const std::string& filename, const std::string& path, FileDownloadCallback callback);

private:
    void sessionFinished(const boost::system::error_code& ec, DownloadSessionPtr session);
    void onRead(const boost::system::error_code& ec, std::size_t bytes_transferred, DownloadSessionPtr session);
    void onWrite(const boost::system::error_code& ec, std::size_t bytes_transferred, DownloadSessionPtr session);
    void onConnect(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator, DownloadSessionPtr session);
    void onResolve(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::results_type results, DownloadSessionPtr session);

private:
    boost::asio::io_context m_ioc;
    boost::asio::io_context::work m_work;
    std::unique_ptr<JoinableThread> m_workThread;
};
